/*****************************************************************************************************************************
This is a project on 3D_SSDsim, based on ssdsim under the framework of the completion of structures, the main function:
1.Support for 3D commands, for example:mutli plane\interleave\copyback\program suspend/Resume..etc
2.Multi - level parallel simulation
3.Clear hierarchical interface
4.4-layer structure

FileName�� initialize.c
Author: Zuo Lu 		Version: 1.1	Date:2017/05/12
Description: 
Initialization layer: complete ssd organizational data structure, request queue creation and memory space initialization

History:
<contributor>     <time>        <version>       <desc>									<e-mail>
Zuo Lu	        2017/04/06	      1.0		    Creat 3D_SSDsim							617376665@qq.com
Zuo Lu			2017/05/12		  1.1			Support advanced commands:mutli plane   617376665@qq.com
*****************************************************************************************************************************/

#define _CRTDBG_MAP_ALLOC
 
#include <stdlib.h>
#include <crtdbg.h>
#include "initialize.h"
#include "buffer.h"
#include "interface.h"
#include "ftl.h"
#include "fcl.h"

#define FALSE		0
#define TRUE		1

#define ACTIVE_FIXED 0
#define ACTIVE_ADJUST 1


/************************************************************************
* Compare function for AVL Tree                                        
************************************************************************/
extern int keyCompareFunc(TREE_NODE *p , TREE_NODE *p1)
{
	struct buffer_group *T1=NULL,*T2=NULL;

	T1=(struct buffer_group*)p;
	T2=(struct buffer_group*)p1;


	if(T1->group< T2->group) return 1;
	if(T1->group> T2->group) return -1;

	return 0;
}


extern int freeFunc(TREE_NODE *pNode)
{
	
	if(pNode!=NULL)
	{
		free((void *)pNode);
	}
	
	
	pNode=NULL;
	return 1;
}


/**********   initiation   ******************
*initialize the ssd struct to simulate the ssd hardware
*1.this function allocate memory for ssd structure 
*2.set the infomation according to the parameter file
*******************************************/
struct ssd_info *initiation(struct ssd_info *ssd)
{
	unsigned int x=0,y=0,i=0,j=0,k=0,l=0,m=0,n=0;
	errno_t err;
	char buffer[300];
	struct parameter_value *parameters;
	FILE *fp=NULL;
	
//	printf("input parameter file name:");
//	gets(ssd->parameterfilename);
 	strcpy_s(ssd->parameterfilename,16,"page.parameters");

//	printf("\ninput trace file name:");
//	gets(ssd->tracefilename);
//	strcpy_s(ssd->tracefilename, 50, "16M_2KB_sequence_RandW.ascii");
//	strcpy_s(ssd->tracefilename, 25, "financial2.ascii");
	strcpy_s(ssd->tracefilename, 25, "example.ascii");
//	strcpy_s(ssd->tracefilename,50,"update_16_count.ascii");
//	strcpy_s(ssd->tracefilename,25,"DevDivRelease.ascii");

//	printf("\ninput output file name:");
//	gets(ssd->outputfilename);
	strcpy_s(ssd->outputfilename,7,"ex.out");
//	strcpy_s(ssd->outputfilename,25,"CFS.out");
//	strcpy_s(ssd->outputfilename,25,"DevDivRelease.out");

//	printf("\ninput statistic file name:");
//	gets(ssd->statisticfilename);
	strcpy_s(ssd->statisticfilename,16,"statistic10.dat");
//	strcpy_s(ssd->statisticfilename,16,"CFS.dat");
//	strcpy_s(ssd->statisticfilename,25,"DevDivRelease.dat");

	//strcpy_s(ssd->statisticfilename2 ,16,"statistic2.dat");

	//Import the configuration file for ssd
	parameters=load_parameters(ssd->parameterfilename);
	ssd->parameter=parameters;
	ssd->min_lsn=0x7fffffff;
	ssd->page=ssd->parameter->chip_num*ssd->parameter->die_chip*ssd->parameter->plane_die*ssd->parameter->block_plane*ssd->parameter->page_block;

	//Initializes the global variable for ssd_info
	ssd->make_age_free_page = 0;
	ssd->buffer_full_flag = 0;
	ssd->request_lz_count = 0;
	ssd->trace_over_flag = 0;

	//Initialize the statistical parameters
	initialize_statistic(ssd);

	//Initialize dram_info
	ssd->dram = (struct dram_info *)malloc(sizeof(struct dram_info));
	alloc_assert(ssd->dram,"ssd->dram");
	memset(ssd->dram,0,sizeof(struct dram_info));
	initialize_dram(ssd);

	//Initialize channel_info
	ssd->channel_head=(struct channel_info*)malloc(ssd->parameter->channel_number * sizeof(struct channel_info));
	alloc_assert(ssd->channel_head,"ssd->channel_head");
	memset(ssd->channel_head,0,ssd->parameter->channel_number * sizeof(struct channel_info));
	initialize_channels(ssd );

	printf("\n");
	if((err=fopen_s(&ssd->outputfile,ssd->outputfilename,"w")) != 0)
	{
		printf("the output file can't open\n");
		return NULL;
	}

	printf("\n");
	if((err=fopen_s(&ssd->statisticfile,ssd->statisticfilename,"w"))!=0)
	{
		printf("the statistic file can't open\n");
		return NULL;
	}

	printf("\n");
// 	if((err=fopen_s(&ssd->statisticfile2,ssd->statisticfilename2,"w"))!=0)
// 	{
// 		printf("the second statistic file can't open\n");
// 		return NULL;
// 	}

	fprintf(ssd->outputfile,"parameter file: %s\n",ssd->parameterfilename); 
	fprintf(ssd->outputfile,"trace file: %s\n",ssd->tracefilename);
	fprintf(ssd->statisticfile,"parameter file: %s\n",ssd->parameterfilename); 
	fprintf(ssd->statisticfile,"trace file: %s\n",ssd->tracefilename);
	
	fflush(ssd->outputfile);
	fflush(ssd->statisticfile);

	if((err=fopen_s(&fp,ssd->parameterfilename,"r"))!=0)
	{
		printf("\nthe parameter file can't open!\n");
		return NULL;
	}

	//fp=fopen(ssd->parameterfilename,"r");

	fprintf(ssd->outputfile,"-----------------------parameter file----------------------\n");
	fprintf(ssd->statisticfile,"-----------------------parameter file----------------------\n");
	while(fgets(buffer,300,fp))
	{
		fprintf(ssd->outputfile,"%s",buffer);
		fflush(ssd->outputfile);
		fprintf(ssd->statisticfile,"%s",buffer);
		fflush(ssd->statisticfile);
	}

	fprintf(ssd->outputfile,"\n");
	fprintf(ssd->outputfile,"-----------------------simulation output----------------------\n");
	fflush(ssd->outputfile);

	fprintf(ssd->statisticfile,"\n");
	fprintf(ssd->statisticfile,"-----------------------simulation output----------------------\n");
	fflush(ssd->statisticfile);

	fclose(fp);
	printf("initiation is completed!\n");

	return ssd;
}



void initialize_statistic(struct ssd_info * ssd)
{
	//Initialize parameters
	ssd->read_count = 0;
	ssd->update_read_count = 0;
	ssd->gc_read_count = 0;
	ssd->program_count = 0;
	ssd->pre_all_write = 0;
	ssd->update_write_count = 0;
	ssd->gc_write_count = 0;
	ssd->erase_count = 0;
	ssd->direct_erase_count = 0;
	ssd->m_plane_read_count = 0;
	ssd->read_request_count = 0;
	ssd->write_flash_count = 0;
	ssd->write_request_count = 0;
	ssd->read_request_count = 0;
	ssd->ave_read_size = 0.0;
	ssd->ave_write_size = 0.0;
}


struct dram_info * initialize_dram(struct ssd_info * ssd)
{
	unsigned int page_num;

	struct dram_info *dram=ssd->dram;
	dram->dram_capacity = ssd->parameter->dram_capacity;		
	dram->buffer = (tAVLTree *)avlTreeCreate((void*)keyCompareFunc , (void *)freeFunc);
	dram->buffer->max_buffer_sector=ssd->parameter->dram_capacity/SECTOR; //512

	dram->map = (struct map_info *)malloc(sizeof(struct map_info));
	alloc_assert(dram->map,"dram->map");
	memset(dram->map,0, sizeof(struct map_info));

	page_num = ssd->parameter->page_block*ssd->parameter->block_plane*ssd->parameter->plane_die*ssd->parameter->die_chip*ssd->parameter->chip_num;

	dram->map->map_entry = (struct entry *)malloc(sizeof(struct entry) * page_num); 
	alloc_assert(dram->map->map_entry,"dram->map->map_entry");
	memset(dram->map->map_entry,0,sizeof(struct entry) * page_num);
	
	return dram;
}

struct page_info * initialize_page(struct page_info * p_page )
{
	p_page->valid_state =0;
	p_page->free_state = PG_SUB;
	p_page->lpn = -1;
	p_page->written_count=0;
	return p_page;
}

struct blk_info * initialize_block(struct blk_info * p_block,struct parameter_value *parameter)
{
	unsigned int i;
	struct page_info * p_page;
	
	p_block->erase_count = 0;
	p_block->page_read_count = 0;
	p_block->page_write_count = 0;
	p_block->pre_write_count = 0;

	p_block->free_page_num = parameter->page_block;	// all pages are free
	p_block->last_write_page = -1;	// no page has been programmed

	p_block->page_head = (struct page_info *)malloc(parameter->page_block * sizeof(struct page_info));

	alloc_assert(p_block->page_head,"p_block->page_head");
	memset(p_block->page_head,0,parameter->page_block * sizeof(struct page_info));

	for(i = 0; i<parameter->page_block; i++)
	{
		p_page = &(p_block->page_head[i]);
		initialize_page(p_page );
	}
	return p_block;

}

struct plane_info * initialize_plane(struct plane_info * p_plane,struct parameter_value *parameter )
{
	unsigned int i;
	struct blk_info * p_block;
	p_plane->add_reg_ppn = -1;  //Plane address register additional register -1 means no data
	p_plane->free_page=parameter->block_plane*parameter->page_block;
	p_plane->plane_read_count = 0;
	p_plane->plane_program_count = 0;
	p_plane->plane_erase_count = 0;
	p_plane->pre_plane_write_count = 0;

	p_plane->blk_head = (struct blk_info *)malloc(parameter->block_plane * sizeof(struct blk_info));
	alloc_assert(p_plane->blk_head,"p_plane->blk_head");
	memset(p_plane->blk_head,0,parameter->block_plane * sizeof(struct blk_info));

	for(i = 0; i<parameter->block_plane; i++)
	{
		p_block = &(p_plane->blk_head[i]);
		initialize_block( p_block ,parameter);			
	}
	return p_plane;
}

struct die_info * initialize_die(struct die_info * p_die,struct parameter_value *parameter,long long current_time )
{
	unsigned int i;
	struct plane_info * p_plane;

	p_die->die_read_count = 0;
	p_die->die_program_count = 0;
	p_die->die_erase_count = 0;
	p_die->token=0;
		
	p_die->plane_head = (struct plane_info*)malloc(parameter->plane_die * sizeof(struct plane_info));
	alloc_assert(p_die->plane_head,"p_die->plane_head");
	memset(p_die->plane_head,0,parameter->plane_die * sizeof(struct plane_info));

	for (i = 0; i<parameter->plane_die; i++)
	{
		p_plane = &(p_die->plane_head[i]);
		initialize_plane(p_plane,parameter );
	}

	return p_die;
}

struct chip_info * initialize_chip(struct chip_info * p_chip,struct parameter_value *parameter,long long current_time )
{
	unsigned int i;
	struct die_info *p_die;
	
	p_chip->current_state = CHIP_IDLE;
	p_chip->next_state = CHIP_IDLE;
	p_chip->current_time = current_time;
	p_chip->next_state_predict_time = 0;			
	p_chip->die_num = parameter->die_chip;
	p_chip->plane_num_die = parameter->plane_die;
	p_chip->block_num_plane = parameter->block_plane;
	p_chip->page_num_block = parameter->page_block;
	p_chip->subpage_num_page = parameter->subpage_page;
	p_chip->ers_limit = parameter->ers_limit;
	p_chip->token=0;
	p_chip->ac_timing = parameter->time_characteristics;		
	p_chip->chip_read_count = 0;
	p_chip->chip_program_count = 0;
	p_chip->chip_erase_count = 0;

	p_chip->die_head = (struct die_info *)malloc(parameter->die_chip * sizeof(struct die_info));
	alloc_assert(p_chip->die_head,"p_chip->die_head");
	memset(p_chip->die_head,0,parameter->die_chip * sizeof(struct die_info));

	for (i = 0; i<parameter->die_chip; i++)
	{
		p_die = &(p_chip->die_head[i]);
		initialize_die( p_die,parameter,current_time );	
	}

	return p_chip;
}

struct ssd_info * initialize_channels(struct ssd_info * ssd )
{
	unsigned int i,j;
	struct channel_info * p_channel;
	struct chip_info * p_chip;

	// set the parameter of each channel
	for (i = 0; i< ssd->parameter->channel_number; i++)
	{
		ssd->channel_head[i].channel_read_count = 0;
		ssd->channel_head[i].channel_program_count = 0;
		ssd->channel_head[i].channel_erase_count = 0;
		p_channel = &(ssd->channel_head[i]);
		p_channel->chip = ssd->parameter->chip_channel[i];
		p_channel->current_state = CHANNEL_IDLE;
		p_channel->next_state = CHANNEL_IDLE;
		
		p_channel->chip_head = (struct chip_info *)malloc(ssd->parameter->chip_channel[i]* sizeof(struct chip_info));
		alloc_assert(p_channel->chip_head,"p_channel->chip_head");
		memset(p_channel->chip_head,0,ssd->parameter->chip_channel[i]* sizeof(struct chip_info));

		for (j = 0; j< ssd->parameter->chip_channel[i]; j++)
		{
			p_chip = &(p_channel->chip_head[j]);
			initialize_chip(p_chip,ssd->parameter,ssd->current_time );
		}
	}

	return ssd;
}


struct parameter_value *load_parameters(char parameter_file[30])
{
	FILE * fp;
	FILE * fp1;
	FILE * fp2;
	errno_t ferr;
	struct parameter_value *p;
	char buf[BUFSIZE];
	int i;
	int pre_eql,next_eql;
	int res_eql;
	char *ptr;

	p = (struct parameter_value *)malloc(sizeof(struct parameter_value));
	alloc_assert(p,"parameter_value");
	memset(p,0,sizeof(struct parameter_value));
	p->queue_length=4;
	memset(buf,0,BUFSIZE);
		
	if((ferr = fopen_s(&fp,parameter_file,"r"))!= 0)
	{	
		printf("the file parameter_file error!\n");	
		return p;
	}
	if((ferr = fopen_s(&fp1,"parameters_name.txt","w"))!= 0)
	{	
		printf("the file parameter_name error!\n");	
		return p;
	}
	if((ferr = fopen_s(&fp2,"parameters_value.txt","w")) != 0)
	{	
		printf("the file parameter_value error!\n");	
		return p;
	}
    


	while(fgets(buf,200,fp)){
		if(buf[0] =='#' || buf[0] == ' ') continue;
		ptr=strchr(buf,'=');
		if(!ptr) continue; 
		
		pre_eql = ptr - buf;
		next_eql = pre_eql + 1;

		while(buf[pre_eql-1] == ' ') pre_eql--;
		buf[pre_eql] = 0;
		if((res_eql=strcmp(buf,"chip number")) ==0){			
			sscanf(buf + next_eql,"%d",&p->chip_num);           //The number of chips
		}else if((res_eql=strcmp(buf,"dram capacity")) ==0){
			sscanf(buf + next_eql,"%d",&p->dram_capacity);      //The size of the cache, the unit is byte
		}else if((res_eql=strcmp(buf,"channel number")) ==0){
			sscanf(buf + next_eql,"%d",&p->channel_number);		//The number of channels
		}else if((res_eql=strcmp(buf,"die number")) ==0){
			sscanf(buf + next_eql,"%d",&p->die_chip);			//The number of die
		}else if((res_eql=strcmp(buf,"plane number")) ==0){
			sscanf(buf + next_eql,"%d",&p->plane_die);			//The number of planes
		}else if((res_eql=strcmp(buf,"block number")) ==0){
			sscanf(buf + next_eql,"%d",&p->block_plane);		//The number of blocks
		}else if((res_eql=strcmp(buf,"page number")) ==0){
			sscanf(buf + next_eql,"%d",&p->page_block);			//The number of pages
		}else if((res_eql=strcmp(buf,"subpage page")) ==0){
			sscanf(buf + next_eql,"%d",&p->subpage_page);		//Page contains subpage (number of sectors)
		}else if((res_eql=strcmp(buf,"page capacity")) ==0){   
			sscanf(buf + next_eql,"%d",&p->page_capacity);		//The size of a page
		}else if((res_eql=strcmp(buf,"subpage capacity")) ==0){
			sscanf(buf + next_eql,"%d",&p->subpage_capacity);   //The size of a subpage (sector)
		}else if((res_eql=strcmp(buf,"t_PROG")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tPROG); //Write time to write flash
		}else if((res_eql=strcmp(buf,"t_DBSY")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tDBSY); 
		}else if((res_eql=strcmp(buf,"t_BERS")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tBERS); // erases the time of a block
		}else if((res_eql=strcmp(buf,"t_CLS")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tCLS); 
		}else if((res_eql=strcmp(buf,"t_CLH")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tCLH); 
		}else if((res_eql=strcmp(buf,"t_CS")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tCS); 
		}else if((res_eql=strcmp(buf,"t_CH")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tCH); 
		}else if((res_eql=strcmp(buf,"t_WP")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tWP); 
		}else if((res_eql=strcmp(buf,"t_ALS")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tALS); 
		}else if((res_eql=strcmp(buf,"t_ALH")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tALH); 
		}else if((res_eql=strcmp(buf,"t_DS")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tDS); 
		}else if((res_eql=strcmp(buf,"t_DH")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tDH); 
		}else if((res_eql=strcmp(buf,"t_WC")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tWC); //Transfer address One byte of time
		}else if((res_eql=strcmp(buf,"t_WH")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tWH); 
		}else if((res_eql=strcmp(buf,"t_ADL")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tADL); 
		}else if((res_eql=strcmp(buf,"t_R")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tR); //The time to read flash
		}else if((res_eql=strcmp(buf,"t_AR")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tAR); 
		}else if((res_eql=strcmp(buf,"t_CLR")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tCLR); 
		}else if((res_eql=strcmp(buf,"t_RR")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tRR); 
		}else if((res_eql=strcmp(buf,"t_RP")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tRP); 
		}else if((res_eql=strcmp(buf,"t_WB")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tWB); 
		}else if((res_eql=strcmp(buf,"t_RC")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tRC); //The time it takes to transfer data one byte
		}else if((res_eql=strcmp(buf,"t_REA")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tREA); 
		}else if((res_eql=strcmp(buf,"t_CEA")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tCEA); 
		}else if((res_eql=strcmp(buf,"t_RHZ")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tRHZ); 
		}else if((res_eql=strcmp(buf,"t_CHZ")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tCHZ); 
		}else if((res_eql=strcmp(buf,"t_RHOH")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tRHOH); 
		}else if((res_eql=strcmp(buf,"t_RLOH")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tRLOH); 
		}else if((res_eql=strcmp(buf,"t_COH")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tCOH); 
		}else if((res_eql=strcmp(buf,"t_REH")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tREH); 
		}else if((res_eql=strcmp(buf,"t_IR")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tIR); 
		}else if((res_eql=strcmp(buf,"t_RHW")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tRHW); 
		}else if((res_eql=strcmp(buf,"t_WHR")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tWHR); 
		}else if((res_eql=strcmp(buf,"t_RST")) ==0){
			sscanf(buf + next_eql,"%d",&p->time_characteristics.tRST); 
		}else if((res_eql=strcmp(buf,"erase limit")) ==0){
			sscanf(buf + next_eql,"%d",&p->ers_limit);					//The number of times each block can be erased
		}else if((res_eql=strcmp(buf,"address mapping")) ==0){
			sscanf(buf + next_eql,"%d",&p->address_mapping);			//Address type (1: page; 2: block; 3: fast)
		}else if((res_eql=strcmp(buf,"wear leveling")) ==0){
			sscanf(buf + next_eql,"%d",&p->wear_leveling);				//Supports WL mode
		}else if((res_eql=strcmp(buf,"gc")) ==0){
			sscanf(buf + next_eql,"%d",&p->gc);							//Gc strategy, the general gc strategy, using the gc_threshold as a threshold, the active write strategy, that can be interrupted gc, need to use gc_hard_threshold hard threshold
		}else if((res_eql=strcmp(buf,"overprovide")) ==0){ 
			sscanf(buf + next_eql,"%f",&p->overprovide);                //The size of the op space
		}else if((res_eql=strcmp(buf,"gc threshold")) ==0){
			sscanf(buf + next_eql,"%f",&p->gc_threshold);               //Gc threshold setting
		}else if((res_eql=strcmp(buf,"buffer management")) ==0){
			sscanf(buf + next_eql,"%d",&p->buffer_management);          //Whether to support data cache
		}else if((res_eql=strcmp(buf,"scheduling algorithm")) ==0){
			sscanf(buf + next_eql,"%d",&p->scheduling_algorithm);       //Scheduling algorithm :FCFS
		}else if((res_eql=strcmp(buf,"gc hard threshold")) ==0){
			sscanf(buf + next_eql,"%f",&p->gc_hard_threshold);          //Gc hard threshold setting for the active write gc strategy to determine the threshold
		}else if((res_eql=strcmp(buf,"allocation")) ==0){
			sscanf(buf + next_eql,"%d",&p->allocation_scheme);		    //Determine the allocation method, 0 that dynamic allocation, that is, dynamic allocation of each channel, the static allocation that according to address allocation
		}else if((res_eql=strcmp(buf, "dynamic_allocation")) == 0){
			sscanf(buf + next_eql, "%d", &p->dynamic_allocation);        //Recorded in the use of which dynamic allocation, 0 that full dynamic, 1 said channel set package, die, plane dynamic, 2 said channel, package, die fixed plane dynamic
		}else if((res_eql=strcmp(buf, "dynamic_allocation_priority")) == 0){
			sscanf(buf + next_eql, "%d", &p->dynamic_allocation_priority);	 //Indicates the priority of the ssd allocation mode, 0 means channel> chip> die> plane, and 1 represents plane> channel> chip> die
		}else if((res_eql=strcmp(buf,"advanced command")) ==0){
			sscanf(buf + next_eql,"%d",&p->advanced_commands);         //Whether to use the advanced command, 0 means not to use. (00001), copyback (00010), two-plane-program (00100), interleave (01000), and two-plane-read (10000) are used respectively, and all use is 11111, both 31       
		}else if((res_eql=strcmp(buf,"advanced command priority2")) ==0){
			sscanf(buf + next_eql,"%d",&p->ad_priority2);				//The priority of the advanced command, 0 that interleave priority higher than the two plane, 1 said two plane priority higher than interleave
		}else if((res_eql=strcmp(buf,"greed CB command")) ==0){
			sscanf(buf + next_eql,"%d",&p->greed_CB_ad);				//Indicates whether greedy use of the copyback advanced command, 0 for no, 1 for greedy use
		}else if((res_eql=strcmp(buf,"greed MPW command")) ==0){
			sscanf(buf + next_eql,"%d",&p->greed_MPW_ad);               //Indicates whether greedy use of multi-plane write advanced command, 0 for no, 1 for greedy use
		}else if((res_eql=strcmp(buf,"aged")) ==0){
			sscanf(buf + next_eql,"%d",&p->aged);                       //1 indicates that the SSD needs to be aged, 0 means that the SSD needs to be kept non-aged
		}else if((res_eql=strcmp(buf,"aged ratio")) ==0){
			sscanf(buf + next_eql,"%f",&p->aged_ratio);                 //Indicates that the SSD needs to be set to invaild in advance for SSD to become aged
		}else if((res_eql=strcmp(buf,"queue_length")) ==0){
			sscanf(buf + next_eql,"%d",&p->queue_length);               //Request the queue depth
		}else if((res_eql=strncmp(buf,"chip number",11)) ==0)
		{
			sscanf(buf+12,"%d",&i);
			sscanf(buf + next_eql,"%d",&p->chip_channel[i]);            //The number of chips on a channel
		}else{
			printf("don't match\t %s\n",buf);
		}
		
		memset(buf,0,BUFSIZE);
		
	}
	fclose(fp);
	fclose(fp1);
	fclose(fp2);

	return p;
}


