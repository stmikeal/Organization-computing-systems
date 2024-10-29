#include "Processor.h"

int sc_main(int, char*[]) {
  sc_core::sc_clock clk("clk", 100, sc_core::SC_NS, 0.5, 0, sc_core::SC_NS, false);
  Processor processor("processor");
  processor.clk_i(clk);
  sc_start(2, sc_core::SC_SEC);
  return 0;
}
