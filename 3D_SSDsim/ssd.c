/*****************************************************************************************************************************
This is a project on 3D_SSDsim, based on ssdsim under the framework of the completion of structures, the main function:
1.Support for 3D commands, for example:mutli plane\interleave\copyback\program suspend/Resume..etc
2.Multi - level parallel simulation
3.Clear hierarchical interface
4.4-layer structure

FileName： ssd.c
Author: Zuo Lu 		Version: 1.9	Date:2017/10/11
Description: System main function c file, Contains the basic flow of simulation.
Mainly includes: initialization, make_aged, pre_process_page three parts

History:
<contributor>     <time>        <version>       <desc>													<e-mail>
Zuo Lu	        2017/04/06	      1.0		    Creat 3D_SSDsim											617376665@qq.com
Zuo Lu			2017/05/12		  1.1			Support advanced commands:mutli plane					617376665@qq.com
Zuo Lu			2017/06/12		  1.2			Support advanced commands:half page read				617376665@qq.com
Zuo Lu			2017/06/16		  1.3			Support advanced commands:one shot program				617376665@qq.com
Zuo Lu			2017/06/22		  1.4			Support advanced commands:one shot read					617376665@qq.com
Zuo Lu			2017/07/07		  1.5			Support advanced commands:erase suspend/resume			617376665@qq.com
Zuo Lu			2017/07/24		  1.6			Support static allocation strategy						617376665@qq.com
Zuo Lu			2017/07/27		  1.7			Support hybrid allocation strategy						617376665@qq.com
Zuo Lu			2017/08/17		  1.8			Support dynamic stripe allocation strategy				617376665@qq.com
Zuo Lu			2017/10/11		  1.9			Support dynamic OSPA allocation strategy				617376665@qq.com
*****************************************************************************************************************************/

#define _CRTDBG_MAP_ALLOC
 
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <crtdbg.h>  


#include "ssd.h"
#include "initialize.h"
#include "flash.h"
#include "buffer.h"
#include "interface.h"
#include "ftl.h"
#include "fcl.h"

int secno_num_per_page, secno_num_sub_page;
/*
//parameters路径名
char *parameters_file[3] =
{ "page_TDA.parameters",
"page_OSA.parameters",
"page_TSA.parameters"
};

//trace 路径名
char *trace_file[15] =
{
	"exchange.ascii", "fiu_web.ascii", "hm0.ascii", "hm1.ascii", "proj0.ascii", "proj3.ascii", "rsrch0.ascii", "src0.ascii", "src1.ascii", 
    "ts0.ascii", "usr0.ascii", "vps.ascii", "w1.ascii", "w2.ascii", "wdev0.ascii"
};


char *result_file_statistic[3][15] =
{ 
	{ "exchange_TDA.dat", "fiu_web_TDA.dat", "hm0_TDA.dat", "hm1_TDA.dat", "proj0_TDA.dat", "proj3_TDA.dat", "rsrch0_TDA.dat", "src0_TDA.dat", "src1_TDA.dat",
	 "ts0_TDA.dat", "usr0_TDA.dat", "vps_TDA.dat", "w1_TDA.dat", "w2_TDA.dat", "wdev0_TDA.dat"
	},
    
	{ "exchange_OSA.dat", "fiu_web_OSA.dat", "hm0_OSA.dat", "hm1_OSA.dat", "proj0_OSA.dat", "proj3_OSA.dat", "rsrch0_OSA.dat", "src0_OSA.dat", "src1_OSA.dat",
	 "ts0_OSA.dat", "usr0_OSA.dat",  "vps_OSA.dat", "w1_OSA.dat", "w2_OSA.dat", "wdev0_OSA.dat"
	},

	{ "exchange_TSA.dat", "fiu_web_TSA.dat", "hm0_TSA.dat", "hm1_TSA.dat", "proj0_TSA.dat", "proj3_TSA.dat", "rsrch0_TSA.dat", "src0_TSA.dat", "src1_TSA.dat",
	"ts0_TSA.dat", "usr0_TSA.dat", "vps_TSA.dat", "w1_TSA.dat", "w2_TSA.dat", "wdev0_TSA.dat"
	}
};


char *result_file_ex[3][15] =
{ 
	{ "exchange_TDA_ex.dat", "fiu_web_TDA_ex.dat", "hm0_TDA_ex.dat", "hm1_TDA_ex.dat", "proj0_TDA_ex.dat", "proj3_TDA_ex.dat", "rsrch0_TDA_ex.dat", "src0_TDA_ex.dat", "src1_TDA_ex.dat",
	 "ts0_TDA_ex.dat", "usr0_TDA_ex.dat", "vps_TDA_ex.dat", "w1_TDA_ex.dat", "w2_TDA_ex.dat", "wdev0_TDA_ex.dat"
	},

	{ "exchange_OSA_ex.dat", "fiu_web_OSA_ex.dat", "hm0_OSA_ex.dat", "hm1_OSA_ex.dat", "proj0_OSA_ex.dat", "proj3_OSA_ex.dat", "rsrch0_OSA_ex.dat", "src0_OSA_ex.dat", "src1_OSA_ex.dat",
	 "ts0_OSA_ex.dat", "usr0_OSA_ex.dat", "vps_OSA_ex.dat", "w1_OSA_ex.dat", "w2_OSA_ex.dat", "wdev0_OSA_ex.dat"
	},

	{ "exchange_TSA_ex.dat", "fiu_web_TSA_ex.dat", "hm0_TSA_ex.dat", "hm1_TSA_ex.dat", "proj0_TSA_ex.dat", "proj3_TSA_ex.dat", "rsrch0_TSA_ex.dat", "src0_TSA_ex.dat", "src1_TSA_ex.dat",
	"ts0_TSA_ex.dat", "usr0_TSA_ex.dat", "vps_TSA_ex.dat", "w1_TSA_ex.dat", "w2_TSA_ex.dat", "wdev0_TSA_ex.dat"
	}
};

char *result_file_die[3][15] =
{ 
	
	{ "exchange_TDA_die.dat", "fiu_web_TDA_die.dat", "hm0_TDA_die.dat", "hm1_TDA_die.dat", "proj0_TDA_die.dat", "proj3_TDA_die.dat", "rsrch0_TDA_die.dat", "src0_TDA_die.dat", "src1_TDA_die.dat",
	 "ts0_TDA_die.dat", "usr0_TDA_die.dat", "vps_TDA_die.dat", "w1_TDA_die.dat", "w2_TDA_die.dat", "wdev0_TDA_die.dat"
	},

	{ "exchange_OSA_die.dat", "fiu_web_OSA_die.dat", "hm0_OSA_die.dat", "hm1_OSA_die.dat", "proj0_OSA_die.dat", "proj3_OSA_die.dat", "rsrch0_OSA_die.dat", "src0_OSA_die.dat", "src1_OSA_die.dat",
	 "ts0_OSA_die.dat", "usr0_OSA_die.dat", "vps_OSA_die.dat", "w1_OSA_die.dat", "w2_OSA_die.dat", "wdev0_OSA_die.dat"
	},

	{ "exchange_TSA_die.dat", "fiu_web_TSA_die.dat", "hm0_TSA_die.dat", "hm1_TSA_die.dat", "proj0_TSA_die.dat", "proj3_TSA_die.dat", "rsrch0_TSA_die.dat", "src0_TSA_die.dat", "src1_TSA_die.dat",
	"ts0_TSA_die.dat", "usr0_TSA_die.dat", "vps_TSA_die.dat", "w1_TSA_die.dat", "w2_TSA_die.dat", "wdev0_TSA_die.dat"
	}
};

char *result_file_size[3][15] =
{
	{ "exchange_TDA_size.dat", "fiu_web_TDA_size.dat", "hm0_TDA_size.dat", "hm1_TDA_size.dat", "proj0_TDA_size.dat", "proj3_TDA_size.dat", "rsrch0_TDA_size.dat", "src0_TDA_size.dat", "src1_TDA_size.dat",
	 "ts0_TDA_size.dat", "usr0_TDA_size.dat", "vps_TDA_size.dat", "w1_TDA_size.dat", "w2_TDA_size.dat", "wdev0_TDA_size.dat"
	},

	{ "exchange_OSA_size.dat", "fiu_web_OSA_size.dat", "hm0_OSA_size.dat", "hm1_OSA_size.dat", "proj0_OSA_size.dat", "proj3_OSA_size.dat", "rsrch0_OSA_size.dat", "src0_OSA_size.dat", "src1_OSA_size.dat",
	 "ts0_OSA_size.dat", "usr0_OSA_size.dat", "vps_OSA_size.dat", "w1_OSA_size.dat", "w2_OSA_size.dat", "wdev0_OSA_size.dat"
	},

	{ "exchange_TSA_size.dat", "fiu_web_TSA_size.dat", "hm0_TSA_size.dat", "hm1_TSA_size.dat", "proj0_TSA_size.dat", "proj3_TSA_size.dat", "rsrch0_TSA_size.dat", "src0_TSA_size.dat", "src1_TSA_size.dat",
	"ts0_TSA_size.dat", "usr0_TSA_size.dat", "vps_TSA_size.dat", "w1_TSA_size.dat", "w2_TSA_size.dat", "wdev0_TSA_size.dat"
	}

};*/
char *parameters_file = "page.parameters";
char *trace_file = "usr2.ascii";
char *result_file_ex = "example_ex.dat";
char *result_file_statistic = "example.dat";


/********************************************************************************************************************************
1，the initiation() used to initialize ssd;
2，the make_aged() used to handle old processing on ssd, Simulation used for some time ssd;
3，the pre_process_page() handle all read request in advance and established lpn<-->ppn of read request in advance ,in order to 
pre-processing trace to prevent the read request is not read the data;
4. the pre_process_write() ensure that up to a valid block contains free page, to meet the actual ssd mechanism after make_aged and pre-process()
5，the simulate() is the actual simulation handles the core functions
6，the statistic_output () outputs the information in the ssd structure to the output file, which outputs statistics and averaging data
7，the free_all_node() release the request for the entire main function
*********************************************************************************************************************************/

void main()
{
	unsigned int i = 0,j = 0;
	struct ssd_info *ssd;

	//for (j = 0; j < 1; j++)
	//{
		j = 1;
		for (i = 0; i < 1; i++)
		{
			//j = 1;
			//i = 1;
			//初始化ssd结构体
			ssd = (struct ssd_info*)malloc(sizeof(struct ssd_info));
			alloc_assert(ssd, "ssd");
			memset(ssd, 0, sizeof(struct ssd_info));

			//输入配置文件参数
			//strcpy_s(ssd->parameterfilename, 50, parameters_file[j]);
			strcpy_s(ssd->parameterfilename, 50, "page.parameters");

			//输入trace文件参数，输出文件名
			/*strcpy_s(ssd->tracefilename, 50, trace_file[i]);
			strcpy_s(ssd->outputfilename, 50, result_file_ex[j][i]);
			strcpy_s(ssd->statisticfilename, 50, result_file_statistic[j][i]);
//			strcpy_s(ssd->statistic_time_filename, 50, result_file_die[j][i]);
//			strcpy_s(ssd->statistic_size_filename, 50, result_file_size[j][i]);*/
			strcpy_s(ssd->tracefilename, 50, trace_file);
			strcpy_s(ssd->outputfilename, 50, result_file_ex);
			strcpy_s(ssd->statisticfilename, 50, result_file_statistic);

			printf("tracefile:%s begin simulate-------------------------\n", ssd->tracefilename);
			//getchar();

			//开始对当前trace进行仿真
			tracefile_sim(ssd);

			//仿真结束，释放所有的节点
			printf("tracefile:%s end simulate---------------------------\n\n\n", ssd->tracefilename);
			free_all_node(ssd);
			//getchar();
		}
	//}

	//所有trace跑完停止当前程序
	system("pause");
}


//void main()
void tracefile_sim(struct ssd_info *ssd)
{
	unsigned  int i,j,k,p,m,n;
	//struct ssd_info *ssd;

	/*
	int tmp;
	tmp = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmp |= _CRTDBG_CHECK_ALWAYS_DF;
	tmp |= _CRTDBG_DELAY_FREE_MEM_DF;
	_CrtSetDbgFlag(tmp);
	*/
	#ifdef DEBUG
	printf("enter main\n"); 
	#endif
	/*
	ssd = (struct ssd_info*)malloc(sizeof(struct ssd_info));
	alloc_assert(ssd, "ssd");
	memset(ssd, 0, sizeof(struct ssd_info));
	*/


	ssd=initiation(ssd);

	make_aged(ssd);

	pre_process_page(ssd);

	if (ssd->parameter->aged == 1)
		pre_process_write(ssd);   

	//After the preprocessing is complete, the page offset address of each plane should be guaranteed to be consistent
	for (i=0;i<ssd->parameter->channel_number;i++)
	{
		for (m = 0; m < ssd->parameter->chip_channel[i]; m++)
		{
			for (j = 0; j < ssd->parameter->die_chip; j++)
			{
				for (k = 0; k < ssd->parameter->plane_die; k++)
				{	
					/*
					for (p = 0; p < ssd->parameter->block_plane; p++)
					{
						printf("%d,0,%d,%d,%d,%d:  %5d\n", i, m, j, k, p, ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].last_write_page);
					}
					*/
					printf("%d,0,%d,%d,%d:  %5d\n", i, m, j, k, ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].free_page);
				}
			}
		}
	}

	fprintf(ssd->outputfile,"\t\t\t\t\t\t\t\t\tOUTPUT\n");
	fprintf(ssd->outputfile,"****************** TRACE INFO ******************\n");

	ssd=simulate(ssd);
	statistic_output(ssd);  
	//free_all_node(ssd);

	printf("\n");
	printf("the simulation is completed!\n");

	//system("pause");
 	_CrtDumpMemoryLeaks();  //Memory leak detection
}


/******************simulate() *********************************************************************
*Simulation () is the core processing function, the main implementation of the features include:
*1, get_requests() :Get a request from the trace file and hang to ssd-> request
*2，buffer_management()/distribute()/no_buffer_distribute() :Make read and write requests through 
the buff layer processing, resulting in read and write sub_requests,linked to ssd-> channel or ssd

*3，process() :Follow these events to handle these read and write sub_requests
*4，trace_output() :Process the completed request, and count the simulation results
**************************************************************************************************/
struct ssd_info *simulate(struct ssd_info *ssd)
{
	int flag=1;
	double output_step=0;
	unsigned int a=0,b=0;
	errno_t err;
	unsigned int i, j, k,m,p;

	printf("\n");
	printf("begin simulating.......................\n");
	printf("\n");
	printf("\n");
	printf("   ^o^    OK, please wait a moment, and enjoy music and coffee   ^o^    \n");

	//Empty the allocation token in pre_process()
	ssd->token = 0;
	for (i = 0; i < ssd->parameter->channel_number; i++)
	{
		for (j = 0; j < ssd->parameter->chip_channel[i]; j++)
		{
			for (k = 0; k < ssd->parameter->die_chip; k++)
			{
				ssd->channel_head[i].chip_head[j].die_head[k].token = 0;
			}
			ssd->channel_head[i].chip_head[j].token = 0;
		}
		ssd->channel_head[i].token = 0;
	}


	if((err=fopen_s(&(ssd->tracefile),ssd->tracefilename,"r"))!=0)
	{  
		printf("the trace file can't open\n");
		return NULL;
	}

	/*fprintf(ssd->outputfile,"      arrive           lsn     size ope     begin time    response time    process time\n");	
	fflush(ssd->outputfile);*/

	while(flag!=100)      
	{        
		/*interface layer*/
		flag = get_requests(ssd);        
		
		/*buffer layer*/
		if (flag == 1 || (flag == 0 && ssd->request_work != NULL))
		{   
			if (ssd->parameter->dram_capacity!=0)
			{
				if (ssd->buffer_full_flag == 0)				//buffer don't block,it can be handle.
				{
					buffer_management(ssd);
				}
			} 
			else
			{
				no_buffer_distribute(ssd);
			}

			if (ssd->request_work->cmplt_flag == 1)
			{
				if (ssd->request_work != ssd->request_tail)
					ssd->request_work = ssd->request_work->next_node;
				else
					ssd->request_work = NULL;
			}

		}
		
		/*ftl+fcl+flash layer*/
		process(ssd); 


		trace_output(ssd);
	
		if (flag == 0 && ssd->request_queue == NULL)
			flag = 100;
	}

	fclose(ssd->tracefile);
	return ssd;
}


/********************************************************
*the main function :Controls the state change processing 
*of the read request and the write request
*********************************************************/
struct ssd_info *process(struct ssd_info *ssd)
{
	int old_ppn = -1, flag_die = -1;
	unsigned int i,j,k,m,p,chan, random_num;
	unsigned int flag = 0, new_write = 0, chg_cur_time_flag = 1, flag2 = 0, flag_gc = 0;
	__int64 time, channel_time = 0x7fffffffffffffff;

#ifdef DEBUG
	printf("enter process,  current time:%I64u\n", ssd->current_time);
#endif

	/*
	if (ssd->channel_head[0].subs_r_head == NULL && ssd->channel_head[0].subs_r_tail != NULL)
		printf("lz\n");
	*/

	/*********************************************************
	*flag=0, processing read and write sub_requests
	*flag=1, processing gc request
	**********************************************************/
	for (i = 0; i<ssd->parameter->channel_number; i++)
	{
		if ((ssd->channel_head[i].subs_r_head == NULL) && (ssd->channel_head[i].subs_w_head == NULL) && (ssd->subs_w_head == NULL))
		{
			flag = 1;
		}
		else
		{
			flag = 0;
			break;
		}
	}
	if (flag == 1)
	{
		ssd->flag = 1;
		if (ssd->gc_request>0)                                                           
		{
			gc(ssd, 0, 1);                                                                  /*Active gc, require all channels must traverse*/
		}
		return ssd;
	}
	else
	{
		ssd->flag = 0;
	}

	/*********************************************************
	*Gc operation is completed, the read and write state changes
	**********************************************************/
	time = ssd->current_time;
	services_2_r_read(ssd);
	services_2_r_complete(ssd);

	random_num = ssd->token;
	for (chan = 0; chan<ssd->parameter->channel_number; chan++)
	{
		i = (random_num + chan) % ssd->parameter->channel_number;
		flag_gc = 0;																		
		ssd->channel_head[i].channel_busy_flag = 0;		
		if ((ssd->channel_head[i].current_state == CHANNEL_IDLE) || (ssd->channel_head[i].next_state == CHANNEL_IDLE&&ssd->channel_head[i].next_state_predict_time <= ssd->current_time))
		{
			//先处理是否有挂起的gc擦除操作，再处理普通的gc操作
			if (ssd->gc_request>0)
			{
				if (ssd->channel_head[i].gc_command != NULL)
				{
					flag_gc = gc(ssd, i, 0);                                                 //process passive gc ,this channel is busy
				}
				if (flag_gc == 1)															 
				{
					continue;
				}
			}
			
			/*********************************************************
			*1.First read the wait state of the read request, 
			*2.followed by the state of the read flash
			*3.end by processing write sub_request
			**********************************************************/
			services_2_r_wait(ssd, i);							  //flag=1,channel is busy，else idle....                  
			if ((ssd->channel_head[i].channel_busy_flag == 0) && (ssd->channel_head[i].subs_r_head != NULL))					  //chg_cur_time_flag=1,current_time has changed，chg_cur_time_flag=0,current_time has not changed  			
				services_2_r_data_trans(ssd, i);

			//Write request state jump
			if (ssd->channel_head[i].channel_busy_flag == 0)
				services_2_write(ssd, i); 
		}

		/*This section is used to see if the offset addresses in the plane are the same, thus verifying the validity of our code*/
		/*
		for (j = 0; j < ssd->parameter->die_chip; j++)
		{
			for (i = 0; i<ssd->parameter->channel_number; i++)
			{
				for (m = 0; m < ssd->parameter->chip_channel[i]; m++)
				{
					for (k = 0; k < ssd->parameter->plane_die; k++)
					{
						for (p = 0; p < ssd->parameter->block_plane; p++)
						{
							if ((ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].free_page_num > 0) && (ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].free_page_num < ssd->parameter->page_block))
							{
								printf("%d %d %d %d %d,%5d\n", i, m, j, k, p, ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].last_write_page);
								//getchar();
							}
						}
					}
				}
			}
		}
		*/
	}
	ssd->token = (ssd->token + 1) % ssd->parameter->channel_number;

	return ssd;
}



/**********************************************************************
*The trace_output () is executed after all the sub requests of each request 
*are processed by the process () 
*Print the output of the relevant results to the outputfile file
**********************************************************************/
void trace_output(struct ssd_info* ssd){
	int flag = 1;
	__int64 start_time, end_time;
	struct request *req, *pre_node;
	struct sub_request *sub, *tmp;

#ifdef DEBUG
	printf("enter trace_output,  current time:%I64u\n", ssd->current_time);
#endif

	pre_node = NULL;
	req = ssd->request_queue;
	start_time = 0;
	end_time = 0;

	if (req == NULL)
		return;

	while (req != NULL)
	{
		sub = req->subs;
		flag = 1;
		start_time = 0;
		end_time = 0;
		if (req->response_time != 0)
		{
			//fprintf(ssd->outputfile, "%16I64u %10u %6u %2u %16I64u %16I64u %10I64u\n", req->time, req->lsn, req->size, req->operation, req->begin_time, req->response_time, req->response_time - req->time);
			//fflush(ssd->outputfile);

			if (req->response_time - req->begin_time == 0)
			{
				printf("the response time is 0?? \n");
				getchar();
			}

			if (req->operation == READ)
			{
				ssd->read_request_count++;
				ssd->read_avg = ssd->read_avg + (req->response_time - req->time);

				//if (ssd->read_request_count % SAMPLE_SPACE == 0)
				//{
				//	//fprintf(ssd->statisticfile_size, "%-12f      %-8u\n", ((ssd->read_request_count*ssd->ave_read_size * 1000 * 1000 * 1000)) / ssd->read_avg, ssd->read_request_count);
				//	fprintf(ssd->statisticfile_size, "%-12llu      %-8lu\n", (ssd->read_avg / ssd->read_request_count), ssd->read_request_count);
				//	fflush(ssd->statisticfile_size);
				//}
			}
			else
			{
				ssd->write_request_count++;
				ssd->write_avg = ssd->write_avg + (req->response_time - req->time);
			}

			if (pre_node == NULL)
			{
				if (req->next_node == NULL)
				{
					free(req->need_distr_flag);
					req->need_distr_flag = NULL;
					free(req);
					req = NULL;
					ssd->request_queue = NULL;
					ssd->request_tail = NULL;
					ssd->request_queue_length--;
				}
				else
				{
					ssd->request_queue = req->next_node;
					pre_node = req;
					req = req->next_node;
					free(pre_node->need_distr_flag);
					pre_node->need_distr_flag = NULL;
					free((void *)pre_node);
					pre_node = NULL;
					ssd->request_queue_length--;
				}
			}
			else
			{
				if (req->next_node == NULL)
				{
					pre_node->next_node = NULL;
					free(req->need_distr_flag);
					req->need_distr_flag = NULL;
					free(req);
					req = NULL;
					ssd->request_tail = pre_node;
					ssd->request_queue_length--;
				}
				else
				{
					pre_node->next_node = req->next_node;
					free(req->need_distr_flag);
					req->need_distr_flag = NULL;
					free((void *)req);
					req = pre_node->next_node;
					ssd->request_queue_length--;
				}
			}
		}
		else
		{
			flag = 1;
			while (sub != NULL)
			{
				if (start_time == 0)
					start_time = sub->begin_time;
				if (start_time > sub->begin_time)
					start_time = sub->begin_time;
				if (end_time < sub->complete_time)
					end_time = sub->complete_time;
				if ((sub->current_state == SR_COMPLETE) || ((sub->next_state == SR_COMPLETE) && (sub->next_state_predict_time <= ssd->current_time)))	// if any sub-request is not completed, the request is not completed
				{
					sub = sub->next_subs;
				}
				else
				{
					flag = 0;
					break;
				}

			}

			if (flag == 1)
			{
				//fprintf(ssd->outputfile, "%16I64u %10u %6u %2u %16I64u %16I64u %10I64u\n", req->time, req->lsn, req->size, req->operation, start_time, end_time, end_time - req->time);
				//fflush(ssd->outputfile);

				if (end_time - start_time == 0)
				{
					printf("the response time is 0?? \n");
					getchar();
				}

				if (req->operation == READ)
				{
					ssd->read_request_count++;
					ssd->read_avg = ssd->read_avg + (end_time - req->time);

					//if (ssd->read_request_count % SAMPLE_SPACE == 0)
					//{
					//	//fprintf(ssd->statisticfile_size, "%-12f      %-8u\n", ((ssd->read_request_count*ssd->ave_read_size * 1000 * 1000 * 1000)) / ssd->read_avg, ssd->read_request_count);
					//	fprintf(ssd->statisticfile_size, "%-12llu      %-8lu\n", (ssd->read_avg / ssd->read_request_count), ssd->read_request_count);
					//	fflush(ssd->statisticfile_size);
					//}
				}
				else
				{
					ssd->write_request_count++;
					ssd->write_avg = ssd->write_avg + (end_time - req->time);
				}

				while (req->subs != NULL)
				{
					tmp = req->subs;
					req->subs = tmp->next_subs;
					if (tmp->update != NULL)
					{
						free(tmp->update->location);
						tmp->update->location = NULL;
						free(tmp->update);
						tmp->update = NULL;
					}
					free(tmp->location);
					tmp->location = NULL;
					free(tmp);
					tmp = NULL;

				}

				if (pre_node == NULL)
				{
					if (req->next_node == NULL)
					{
						free(req->need_distr_flag);
						req->need_distr_flag = NULL;
						free(req);
						req = NULL;
						ssd->request_queue = NULL;
						ssd->request_tail = NULL;
						ssd->request_queue_length--;
					}
					else
					{
						ssd->request_queue = req->next_node;
						pre_node = req;
						req = req->next_node;
						free(pre_node->need_distr_flag);
						pre_node->need_distr_flag = NULL;
						free(pre_node);
						pre_node = NULL;
						ssd->request_queue_length--;
					}
				}
				else
				{
					if (req->next_node == NULL)
					{
						pre_node->next_node = NULL;
						free(req->need_distr_flag);
						req->need_distr_flag = NULL;
						free(req);
						req = NULL;
						ssd->request_tail = pre_node;
						ssd->request_queue_length--;
					}
					else
					{
						pre_node->next_node = req->next_node;
						free(req->need_distr_flag);
						req->need_distr_flag = NULL;
						free(req);
						req = pre_node->next_node;
						ssd->request_queue_length--;
					}

				}
			}
			else
			{
				pre_node = req;
				req = req->next_node;
			}
		}
	}
}


/*******************************************************************************
*statistic_output() output processing of a request after the relevant processing information
*1，Calculate the number of erasures per plane, ie plane_erase and the total number of erasures
*2，Print min_lsn, max_lsn, read_count, program_count and other statistics to the file outputfile.
*3，Print the same information into the file statisticfile
*******************************************************************************/
void statistic_output(struct ssd_info *ssd)
{
	unsigned int lpn_count=0,i,j,k,m,p,erase=0,plane_erase=0;
	unsigned int blk_read = 0, plane_read = 0;
	unsigned int blk_write = 0, plane_write = 0;
	unsigned int pre_plane_write = 0;
	double gc_energy=0.0;
#ifdef DEBUG
	printf("enter statistic_output,  current time:%I64u\n",ssd->current_time);
#endif

	for(i = 0;i<ssd->parameter->channel_number;i++)
	{
		for (p = 0; p < ssd->parameter->chip_channel[i]; p++)
		{
			for (j = 0; j < ssd->parameter->die_chip; j++)
			{
				for (k = 0; k < ssd->parameter->plane_die; k++)
				{
					for (m = 0; m < ssd->parameter->block_plane; m++)
					{
						if (ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].blk_head[m].erase_count > 0)
						{
							ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].plane_erase_count += ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].blk_head[m].erase_count;
						}

						if (ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].blk_head[m].page_read_count > 0)
						{
							ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].plane_read_count += ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].blk_head[m].page_read_count;
						}

						if (ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].blk_head[m].page_write_count > 0)
						{
							ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].plane_program_count += ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].blk_head[m].page_write_count;
						}

						if (ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].blk_head[m].pre_write_count > 0)
						{
							ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].pre_plane_write_count += ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].blk_head[m].pre_write_count;
						}
					}
					fprintf(ssd->outputfile, "the %d channel, %d chip, %d die, %d plane has : ", i, p, j, k);
					fprintf(ssd->outputfile, "%3d erase operations,", ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].plane_erase_count);
					fprintf(ssd->outputfile, "%3d read operations,", ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].plane_read_count);
					fprintf(ssd->outputfile, "%3d write operations,", ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].plane_program_count);
					fprintf(ssd->outputfile, "%3d pre_process write operations\n", ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].pre_plane_write_count);
					
					fprintf(ssd->statisticfile, "the %d channel, %d chip, %d die, %d plane has : ", i, p, j, k);
					fprintf(ssd->statisticfile, "%3d erase operations,", ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].plane_erase_count);
					fprintf(ssd->statisticfile, "%3d read operations,", ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].plane_read_count);
					fprintf(ssd->statisticfile, "%3d write operations,", ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].plane_program_count);
					fprintf(ssd->statisticfile, "%3d pre_process write operations\n", ssd->channel_head[i].chip_head[p].die_head[j].plane_head[k].pre_plane_write_count);
				}
			}
		}
	}

	fprintf(ssd->outputfile,"\n");
	fprintf(ssd->outputfile,"\n");
	fprintf(ssd->outputfile,"---------------------------statistic data---------------------------\n");	 
	fprintf(ssd->outputfile,"min lsn: %13d\n",ssd->min_lsn);	
	fprintf(ssd->outputfile,"max lsn: %13d\n",ssd->max_lsn);
	fprintf(ssd->outputfile,"read count: %13d\n",ssd->read_count);	  
	fprintf(ssd->outputfile,"the read operation leaded by un-covered update count: %13d\n",ssd->update_read_count);
	fprintf(ssd->outputfile, "the read operation leaded by gc read count: %13d\n", ssd->gc_read_count);
	fprintf(ssd->outputfile, "\n");
	fprintf(ssd->outputfile, "program count: %13d\n", ssd->program_count);
	fprintf(ssd->outputfile, "the write operation leaded by pre_process write count: %13d\n", ssd->pre_all_write);
	fprintf(ssd->outputfile, "the write operation leaded by un-covered update count: %13d\n", ssd->update_write_count);
	fprintf(ssd->outputfile, "the write operation leaded by gc read count: %13d\n", ssd->gc_write_count);
	fprintf(ssd->outputfile, "\n");
	fprintf(ssd->outputfile,"erase count: %13d\n",ssd->erase_count);
	fprintf(ssd->outputfile,"direct erase count: %13d\n",ssd->direct_erase_count);
	fprintf(ssd->outputfile, "gc count: %13d\n", ssd->gc_count);

	fprintf(ssd->outputfile, "multi-plane program count: %13d\n", ssd->m_plane_prog_count);
	fprintf(ssd->outputfile, "multi-plane read count: %13d\n", ssd->m_plane_read_count);
	fprintf(ssd->outputfile, "\n");

	fprintf(ssd->outputfile, "mutli plane one shot program count : %13d\n", ssd->mutliplane_oneshot_prog_count);
	fprintf(ssd->outputfile, "one shot program count : %13d\n", ssd->ontshot_prog_count);
	fprintf(ssd->outputfile, "\n");

	fprintf(ssd->outputfile, "half page read count : %13d\n", ssd->half_page_read_count);
	fprintf(ssd->outputfile, "one shot read count : %13d\n", ssd->one_shot_read_count);
	fprintf(ssd->outputfile, "mutli plane one shot read count : %13d\n", ssd->one_shot_mutli_plane_count);
	fprintf(ssd->outputfile, "\n");

	fprintf(ssd->outputfile, "erase suspend count : %13d\n", ssd->suspend_count);
	fprintf(ssd->outputfile, "erase resume  count : %13d\n", ssd->resume_count);
	fprintf(ssd->outputfile, "suspend read  count : %13d\n", ssd->suspend_read_count);
	fprintf(ssd->outputfile, "suspend write  count : %13d\n", ssd->suspend_write_count);
	fprintf(ssd->outputfile, "\n");

	fprintf(ssd->outputfile, "update sub request count : %13d\n", ssd->update_sub_request);
	fprintf(ssd->outputfile,"write flash count: %13d\n",ssd->write_flash_count);
	fprintf(ssd->outputfile, "\n");
	
	fprintf(ssd->outputfile,"read request count: %13d\n",ssd->read_request_count);
	fprintf(ssd->outputfile,"write request count: %13d\n",ssd->write_request_count);
	fprintf(ssd->outputfile, "\n");
	fprintf(ssd->outputfile,"read request average size: %13f\n",ssd->ave_read_size);
	fprintf(ssd->outputfile,"write request average size: %13f\n",ssd->ave_write_size);
	fprintf(ssd->outputfile, "\n");
	fprintf(ssd->outputfile,"read request average response time: %16I64u\n",ssd->read_avg/ssd->read_request_count);
	fprintf(ssd->outputfile,"write request average response time: %16I64u\n",ssd->write_avg/ssd->write_request_count);
	fprintf(ssd->outputfile, "\n");
	fprintf(ssd->outputfile,"buffer read hits: %13d\n",ssd->dram->buffer->read_hit);
	fprintf(ssd->outputfile,"buffer read miss: %13d\n",ssd->dram->buffer->read_miss_hit);
	fprintf(ssd->outputfile,"buffer write hits: %13d\n",ssd->dram->buffer->write_hit);
	fprintf(ssd->outputfile,"buffer write miss: %13d\n",ssd->dram->buffer->write_miss_hit);
	
	fprintf(ssd->outputfile, "update sub request count : %13d\n", ssd->update_sub_request);
	fprintf(ssd->outputfile, "half page read count : %13d\n", ssd->half_page_read_count);
	fprintf(ssd->outputfile, "mutli plane one shot program count : %13d\n", ssd->mutliplane_oneshot_prog_count);
	fprintf(ssd->outputfile, "one shot read count : %13d\n", ssd->one_shot_read_count);
	fprintf(ssd->outputfile, "mutli plane one shot read count : %13d\n", ssd->one_shot_mutli_plane_count);
	fprintf(ssd->outputfile, "erase suspend count : %13d\n", ssd->suspend_count);
	fprintf(ssd->outputfile, "erase resume  count : %13d\n", ssd->resume_count);
	fprintf(ssd->outputfile, "suspend read  count : %13d\n", ssd->suspend_read_count);
	fprintf(ssd->outputfile, "suspend write  count : %13d\n", ssd->suspend_write_count);

	fprintf(ssd->outputfile, "\n");
	fflush(ssd->outputfile);

	fclose(ssd->outputfile);


	fprintf(ssd->statisticfile, "\n");
	fprintf(ssd->statisticfile, "\n");
	fprintf(ssd->statisticfile, "---------------------------statistic data---------------------------\n");
	fprintf(ssd->statisticfile, "min lsn: %13d\n", ssd->min_lsn);
	fprintf(ssd->statisticfile, "max lsn: %13d\n", ssd->max_lsn);
	fprintf(ssd->statisticfile, "read count: %13d\n", ssd->read_count);
	fprintf(ssd->statisticfile, "the read operation leaded by un-covered update count: %13d\n", ssd->update_read_count);
	fprintf(ssd->statisticfile, "the read operation leaded by gc read count: %13d\n", ssd->gc_read_count);
	fprintf(ssd->statisticfile, "\n");
	fprintf(ssd->statisticfile, "program count: %13d\n", ssd->program_count);
	fprintf(ssd->statisticfile, "the write operation leaded by pre_process write count: %13d\n", ssd->pre_all_write);
	fprintf(ssd->statisticfile, "the write operation leaded by un-covered update count: %13d\n", ssd->update_write_count);
	fprintf(ssd->statisticfile, "the write operation leaded by gc read count: %13d\n", ssd->gc_write_count);
	fprintf(ssd->statisticfile, "\n");
	fprintf(ssd->statisticfile,"erase count: %13d\n",ssd->erase_count);	  
	fprintf(ssd->statisticfile,"direct erase count: %13d\n",ssd->direct_erase_count);
	fprintf(ssd->statisticfile, "gc count: %13d\n", ssd->gc_count);
	fprintf(ssd->statisticfile, "\n");

	fprintf(ssd->statisticfile,"multi-plane program count: %13d\n",ssd->m_plane_prog_count);
	fprintf(ssd->statisticfile,"multi-plane read count: %13d\n",ssd->m_plane_read_count);
	fprintf(ssd->statisticfile, "\n");

	fprintf(ssd->statisticfile, "mutli plane one shot program count : %13d\n", ssd->mutliplane_oneshot_prog_count);
	fprintf(ssd->statisticfile, "one shot program count : %13d\n", ssd->ontshot_prog_count);
	fprintf(ssd->statisticfile, "\n");

	fprintf(ssd->statisticfile, "half page read count : %13d\n", ssd->half_page_read_count);
	fprintf(ssd->statisticfile, "one shot read count : %13d\n", ssd->one_shot_read_count);
	fprintf(ssd->statisticfile, "mutli plane one shot read count : %13d\n", ssd->one_shot_mutli_plane_count);
	fprintf(ssd->statisticfile, "\n");

	fprintf(ssd->statisticfile, "erase suspend count : %13d\n", ssd->suspend_count);
	fprintf(ssd->statisticfile, "erase resume  count : %13d\n", ssd->resume_count);
	fprintf(ssd->statisticfile, "suspend read  count : %13d\n", ssd->suspend_read_count);
	fprintf(ssd->outputfile, "suspend write  count : %13d\n", ssd->suspend_write_count);
	fprintf(ssd->statisticfile, "\n");

	fprintf(ssd->statisticfile, "update sub request count : %13d\n", ssd->update_sub_request);
	fprintf(ssd->statisticfile,"write flash count: %13d\n",ssd->write_flash_count);
	fprintf(ssd->statisticfile, "\n");
	
	fprintf(ssd->statisticfile,"read request count: %13d\n",ssd->read_request_count);
	fprintf(ssd->statisticfile, "write request count: %13d\n", ssd->write_request_count);
	fprintf(ssd->statisticfile, "\n");
	fprintf(ssd->statisticfile,"read request average size: %13f\n",ssd->ave_read_size);
	fprintf(ssd->statisticfile,"write request average size: %13f\n",ssd->ave_write_size);
	fprintf(ssd->statisticfile, "\n");
	fprintf(ssd->statisticfile,"read request average response time: %16I64u\n",ssd->read_avg/ssd->read_request_count);
	fprintf(ssd->statisticfile,"write request average response time: %16I64u\n",ssd->write_avg/ssd->write_request_count);
	fprintf(ssd->statisticfile, "\n");
	fprintf(ssd->statisticfile,"buffer read hits: %13d\n",ssd->dram->buffer->read_hit);
	fprintf(ssd->statisticfile,"buffer read miss: %13d\n",ssd->dram->buffer->read_miss_hit);
	fprintf(ssd->statisticfile,"buffer write hits: %13d\n",ssd->dram->buffer->write_hit);
	fprintf(ssd->statisticfile,"buffer write miss: %13d\n",ssd->dram->buffer->write_miss_hit);
	fprintf(ssd->statisticfile, "\n");
	fflush(ssd->statisticfile);

	fclose(ssd->statisticfile);
}




/***********************************************
*free_all_node(): release all applied nodes
************************************************/
void free_all_node(struct ssd_info *ssd)
{
	unsigned int i,j,k,l,n,p;
	struct buffer_group *pt=NULL;
	struct direct_erase * erase_node=NULL;
	for (i=0;i<ssd->parameter->channel_number;i++)
	{
		for (j=0;j<ssd->parameter->chip_channel[0];j++)
		{
			for (k=0;k<ssd->parameter->die_chip;k++)
			{
				for (l=0;l<ssd->parameter->plane_die;l++)
				{
					for (n=0;n<ssd->parameter->block_plane;n++)
					{
						free(ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[n].page_head);
						ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[n].page_head=NULL;
					}
					free(ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head);
					ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head=NULL;
					while(ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].erase_node!=NULL)
					{
						erase_node=ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].erase_node;
						ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].erase_node=erase_node->next_node;
						free(erase_node);
						erase_node=NULL;
					}
				}
				
				free(ssd->channel_head[i].chip_head[j].die_head[k].plane_head);
				ssd->channel_head[i].chip_head[j].die_head[k].plane_head=NULL;
			}
			free(ssd->channel_head[i].chip_head[j].die_head);
			ssd->channel_head[i].chip_head[j].die_head=NULL;
		}
		free(ssd->channel_head[i].chip_head);
		ssd->channel_head[i].chip_head=NULL;
	}
	free(ssd->channel_head);
	ssd->channel_head=NULL;

	avlTreeDestroy( ssd->dram->buffer);
	ssd->dram->buffer=NULL;
	avlTreeDestroy(ssd->dram->command_buffer);
	ssd->dram->command_buffer = NULL;
	
	for (p = 0; p < DIE_NUMBER; p++)
	{
		avlTreeDestroy(ssd->dram->static_die_buffer[p]);
		ssd->dram->static_die_buffer[p] = NULL;
	}

	free(ssd->dram->map->map_entry);
	ssd->dram->map->map_entry=NULL;
	free(ssd->dram->map);
	ssd->dram->map=NULL;
	free(ssd->dram);
	ssd->dram=NULL;
	free(ssd->parameter);
	ssd->parameter=NULL;

	free(ssd);
	ssd=NULL;
}


/*****************************************************************************
*Make_aged () function of the role of death to simulate the real use of a period of time ssd,
******************************************************************************/
struct ssd_info *make_aged(struct ssd_info *ssd)
{
	unsigned int i,j,k,l,m,n,ppn;
	int threshould,flag=0;
    
	if (ssd->parameter->aged==1)
	{
		//Threshold indicates how many pages in a plane need to be set to invaild in advance
		threshould=(int)(ssd->parameter->block_plane*ssd->parameter->page_block*ssd->parameter->aged_ratio);  
		for (i=0;i<ssd->parameter->channel_number;i++)
			for (j=0;j<ssd->parameter->chip_channel[i];j++)
				for (k=0;k<ssd->parameter->die_chip;k++)
					for (l=0;l<ssd->parameter->plane_die;l++)
					{  
						flag=0;
						for (m=0;m<ssd->parameter->block_plane;m++)
						{  
							if (flag>=threshould)
							{
								break;
							}
							//Note that aged_ratio+1 here, that the final conditions to meet the old conditions will remain all the remaining free block.
							for (n=0;n<(ssd->parameter->page_block*ssd->parameter->aged_ratio+1);n++)
							{  
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[m].page_head[n].valid_state=0;        //Indicates that a page is invalid and both the valid and free states are 0
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[m].page_head[n].free_state=0;         //Indicates that a page is invalid and both the valid and free states are 0
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[m].page_head[n].lpn=0;				   //Setting valid_state free_state lpn to 0 means that the page is invalid
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[m].free_page_num--;
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[m].invalid_page_num++;
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[m].last_write_page++;
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].free_page--;
								flag++;

								ppn=find_ppn(ssd,i,j,k,l,m,n);
							
							}
							ssd->make_age_free_page = ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[m].free_page_num;
						} 
					}	 
	}  
	else
	{
		return ssd;
	}

	return ssd;
}



/*********************************************************************************************
*Make free block free_page all invalid, to ensure that up to a valid block contains free page, 
*to meet the actual ssd mechanism.Traverse all the blocks, when the valid block contains the 
*legacy free_page, all the free_page is invalid and the invalid block is placed in the gc chain.
*********************************************************************************************/
struct ssd_info *pre_process_write(struct ssd_info *ssd)
{
	unsigned  int i, j, k, p, m, n;
	struct direct_erase *direct_erase_node, *new_direct_erase;

	for (i = 0; i<ssd->parameter->channel_number; i++)
	{
		for (m = 0; m < ssd->parameter->chip_channel[i]; m++)
		{
			for (j = 0; j < ssd->parameter->die_chip; j++)
			{
				for (k = 0; k < ssd->parameter->plane_die; k++)
				{
					//See if there are free blocks
					for (p = 0; p < ssd->parameter->block_plane; p++)
					{
						if ((ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].free_page_num > 0) && (ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].free_page_num < ssd->parameter->page_block))
						{
							if (ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].free_page_num == ssd->make_age_free_page)			//The current block is invalid page, all set invalid current block
							{
								ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].free_page = ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].free_page - ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].free_page_num;
								ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].free_page_num = 0;
								ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].invalid_page_num = ssd->parameter->page_block;
								ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].last_write_page = ssd->parameter->page_block - 1;

								for (n = 0; n < ssd->parameter->page_block; n++)
								{
									ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].page_head[n].valid_state = 0;        
									ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].page_head[n].free_state = 0;        
									ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].page_head[n].lpn = 0;  
								}
								//All pages are invalid pages, so this block is invalid and added to the gc chain
								new_direct_erase = (struct direct_erase *)malloc(sizeof(struct direct_erase));
								alloc_assert(new_direct_erase, "new_direct_erase");
								memset(new_direct_erase, 0, sizeof(struct direct_erase));

								new_direct_erase->block = p;  
								new_direct_erase->next_node = NULL;
								direct_erase_node = ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].erase_node;
								if (direct_erase_node == NULL)
								{
									ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].erase_node = new_direct_erase;
								}
								else
								{
									new_direct_erase->next_node = ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].erase_node;
									ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].erase_node = new_direct_erase;
								}
							}
							else   //There is an invalid page and a valid page in the current block
							{
								ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].free_page = ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].free_page - ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].free_page_num;
								ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].free_page_num = 0;
								ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].invalid_page_num += ssd->parameter->page_block - (ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].last_write_page + 1);
								ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].last_write_page = ssd->parameter->page_block - 1;

								for (n = (ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].last_write_page + 1); n < ssd->parameter->page_block; n++)
								{
									ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].page_head[n].valid_state = 0;        
									ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].page_head[n].free_state = 0;         
									ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].page_head[n].lpn = 0;  
								}
							}
						}
						//printf("%d,0,%d,%d,%d:%5d\n", i, j, k, p, ssd->channel_head[i].chip_head[m].die_head[j].plane_head[k].blk_head[p].free_page_num);
					}
				}
			}
		}
	}
return ssd;
}


/************************************************
*Assert,open failed，printf“open ... error”
*************************************************/
void file_assert(int error, char *s)
{
	if (error == 0) return;
	printf("open %s error\n", s);
	getchar();
	exit(-1);
}

/*****************************************************
*Assert,malloc failed，printf“malloc ... error”
******************************************************/
void alloc_assert(void *p, char *s)
{
	if (p != NULL) return;
	printf("malloc %s error\n", s);
	getchar();
	exit(-1);
}

/*********************************************************************************
*Assert
*A，trace about time_t，device，lsn，size，ope <0 ，printf“trace error:.....”
*B，trace about time_t，device，lsn，size，ope =0，printf“probable read a blank line”
**********************************************************************************/
void trace_assert(_int64 time_t, int device, unsigned int lsn, int size, int ope)
{
	if (time_t <0 || device < 0 || lsn < 0 || size < 0 || ope < 0)
	{
		printf("trace error:%I64u %d %d %d %d\n", time_t, device, lsn, size, ope);
		getchar();
		exit(-1);
	}
	if (time_t == 0 && device == 0 && lsn == 0 && size == 0 && ope == 0)
	{
		printf("probable read a blank line\n");
		getchar();
	}
}