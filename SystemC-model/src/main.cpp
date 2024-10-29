#include "Processor.h"

int sc_main(int, char*[]) {
  sc_core::sc_clock clk("clk", sc_time(100, SC_NS));
  Processor processor("processor");
  processor.clk_i(clk);
  // printf("main\n");
  sc_start(200, sc_core::SC_SEC);
  return 0;
}
