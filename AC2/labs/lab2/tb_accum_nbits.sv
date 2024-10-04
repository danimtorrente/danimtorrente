module tb_adder_nbits;

    // Signal declaration
    parameter WIDTH = 16;
    parameter TESTSUM = 16;
    logic [WIDTH-1:0] x, y;
    logic [WIDTH:0] tmp;
    logic clk, rst;
    integer i;

    initial begin
        
    end

    // Instance of the accumulator under test
    accum_nbits #(.WIDTH(WIDTH)) accumulator (
      .clk_i(clk),
      .nrst_i(rst),
      .x_i(x),
      .y_o(y)
    );

    // Basic test for all the input combinations
    task basic_functional_testing();
        $display(" ------------------------------------------------");
        $display(" Basic test ");
        $display(" ------------------------------------------------");

        clk = 1; rst = 0; x = 0;
        #500 clk = 0; rst = 1;
        #500
        for (i = 0; i < TESTSUM; i++) begin
            clk = 1;
            x = i[WIDTH-1:0];
            tmp = tmp + i[WIDTH:0];
            #500 clk = 0;
            #500 assert (y[WIDTH-1:0] == tmp[WIDTH-1:0])
                 $display("(%0d) x=%d -> y=%d\n",$time,x,y);
                 else $error("ERROR! (%0d) x=%d -> y=%d\n",$time,x,y);
        end
    endtask

    // Main execution flow of the testbench
    initial begin 
        // wavefrom generation
        $dumpfile("waveform.vcd");
        $dumpvars();

        // Functionality testing of the 
        basic_functional_testing();
        $finish();
    end
                 
endmodule
