
#include "test_bench.h"

static unsigned long start_time = 0;
static unsigned long last_vld_time = 0;

static unsigned int instruction_counter;
static unsigned int flush_counter = 0;
static unsigned int stall_counter = 0;

static perf_metrics_t p;

PLI_INT32 perf_metrics(char* user_data)
{    
  assert(user_data == NULL);
  vpiHandle vhandle, iterator, arg;
  vhandle = vpi_handle(vpiSysTfCall, NULL);

  s_vpi_value inval;

  unsigned int time_h;
  unsigned int time_l;
  unsigned long current_time;

  unsigned int pc_stall;
  unsigned int branch_flush;
  unsigned int instruction0;
  unsigned int instruction1;

  iterator = vpi_iterate(vpiArgument, vhandle);

  arg = vpi_scan(iterator);
  inval.format = vpiTimeVal;
  vpi_get_value(arg, &inval);
  time_h = inval.value.time->high;
  time_l = inval.value.time->low;
  current_time = time_h;
  current_time = (current_time << BITS_IN_INT) | time_l;
  
  arg = vpi_scan(iterator);
  inval.format = vpiVectorVal;
  vpi_get_value(arg, &inval);
  if((inval.value.vector[0].aval == 1) && (inval.value.vector[0].bval == 0))
  {
    stall_counter++;
  }

  arg = vpi_scan(iterator);
  inval.format = vpiVectorVal;
  vpi_get_value(arg, &inval);
  if((inval.value.vector[0].aval == 1) && (inval.value.vector[0].bval == 0))
  {
    flush_counter++;
  }

  arg = vpi_scan(iterator);
  inval.format = vpiVectorVal;
  vpi_get_value(arg, &inval);
  if((inval.value.vector[0].aval > 0) && (inval.value.vector[0].bval == 0))
  {
    last_vld_time = current_time;
    instruction_counter++;
  }

  arg = vpi_scan(iterator);
  inval.format = vpiVectorVal;
  vpi_get_value(arg, &inval);
  if((inval.value.vector[0].aval > 0) && (inval.value.vector[0].bval == 0))
  {
    last_vld_time = current_time;
    instruction_counter++;
  }

  if(start_time == 0)
  {
    start_time = current_time;
  }

  //printf("time: %lu flush: %u stall: %u\n", start_time - last_vld_time, flush_counter, stall_counter);

  return 0;

}

perf_metrics_t* get_perf_metrics()
{
  p.ipc =  ((float)instruction_counter / (last_vld_time - start_time)) * 10;
  p.stall_count = stall_counter;
  p.flush_count = flush_counter;
  return &p;
}

void clear_perf_metrics()
{
  start_time = 0;
  last_vld_time = 0;
  instruction_counter = 0;
  stall_counter = 0;
  flush_counter = 0;
}






