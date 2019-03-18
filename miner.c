/* miner.c  The Block Miner  -- Child Process
 *
 * Copyright (c) 2018 by Adequate Systems, LLC.  All Rights Reserved.
 * See LICENSE.PDF   **** NO WARRANTY ****
 *
 * The Mochimo Project System Software
 *
 * Date: 13 January 2018
 *
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "DKatNetGetWork.h"

#ifdef CUDANODE
extern int trigg_init_cuda(byte difficulty, byte *blockNumber);
extern void trigg_free_cuda();
extern char *trigg_generate_cuda(byte *mroot, unsigned long long *nHaiku);
#endif

/* miner blockin blockout -- child process */
//int miner(char *blockin, char *blockout)
void miner()
{
   FILE *fp;
   byte *ptr;
   SHA256_CTX bctx;  /* to resume entire block hash after bcon.c */
   char *haiku;
   time_t htime;
   unsigned long long hcount, hps, total_hcount;
   word32 temp[3];
   int initGPU;
   struct timespec chill = {0,Dynasleep*1000L};
    srand(time(NULL));
      temp[0] = rand();
      temp[1] = rand();
      temp[2] = rand();
    C_DKNGetWork* pool;
    pool = C_DKNGetWorkFactory("0x0eddc33b0337d63a519b84b73eac4f755a17345afe8cdc148bb05c9499288e55#minicuda", "MCM", "http://mcmpool.pw:8085");
    struct work new_work;
    new_work.work = NULL;
    new_work.res = NULL;
    while (C_IsReady(pool) == 0)
    {
        sleep(1);
    }
    int have_new_work = 0;
   for( ;; sleep(10))
   {
       if (C_HaveNewWork(pool) || have_new_work)
       {
           C_GetWork(pool, &new_work);
           C_Decode64(&new_work);
           new_work.diff -= 16;
       }

      initGPU = -1;
      initGPU = trigg_init_cuda(((byte*)&new_work.diff)[0], ((byte*)&new_work.nblock)); //TBI
      if(initGPU==-1)
      {
         error("Cuda initialization failed. Check nvidia-smi");
         trigg_free_cuda();
         break;
      }
      if(initGPU<1 || initGPU>64) {
         error("Unsupported number of GPUs detected -> %d",initGPU);
         trigg_free_cuda();
         break;
      }

      /* Traverse all TRIGG links to build the
       * solution chain with trigg_generate()...
       */
      for(haiku = NULL, htime = time(NULL), hcount = 0; ;)
      {
         if(haiku != NULL) break;
         if (C_HaveNewWork(pool))
         {
             have_new_work = 1;
             break;
         }
         haiku = trigg_generate_cuda(new_work.decoded_data, &hcount);
         if(total_hcount == hcount) nanosleep(&chill, NULL);
         else total_hcount = hcount;
         if (hcount > 1e10)
         {
             htime = time(NULL) - htime;
             if(htime == 0) htime = 1;
             hps = hcount / htime;
             htime= time(NULL);
             hcount = 0;
             printf("Total hashrate: %.2f \n", ((float)(hps)/1e9f));
         }
      }

      /* Free CUDA specific memory allocations */
      trigg_free_cuda();

      /* Calculate and write Haiku/s to disk */
      htime = time(NULL) - htime;
      if(htime == 0) htime = 1;
      hps = hcount / htime;
      write_data(&hps, 8, "hps.dat");  /* unsigned long haiku per second */
      if (have_new_work || haiku == NULL)
      {
          continue;
      }

      /* Block validation check */
      if (!trigg_check(new_work.decoded_data, new_work.diff, (byte*)(&new_work.nblock))) // TBI
      {
         printf("ERROR - Block is not valid\n");
         break;
      }
      printf("%s \n",haiku);
      C_SubmitWork(pool, &new_work);
   }  /* end for  */
done:
   getrand2(temp, &temp[1], &temp[2]);
   write_data(&temp, 12, "mseed.dat");   /* maintain rand2() sequence */
   if(Trace) plog("Miner exiting...");
}  /* end miner() */


/* Start the miner as a child process */
int start_miner(void)
{
   miner();
   exit(0);
}
