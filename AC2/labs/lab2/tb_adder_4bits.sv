module tb_adder_4bits;

    // Signal declaration
    parameter WIDTH = 4;
    parameter TESTSUM = 16;
    logic [WIDTH-1:0] x, y, s;
    logic [WIDTH:0] tmp;
    logic c_in, c_out;
    logic clk;
    integer i, j;

    initial begin
        
    end

    // Instance of the adder_4bits under test
    adder_4bits adder (
      .x_i(x),
      .y_i(y),
      .c_i(c_in),
      .s_o(s),
      .c_o(c_out)
    );

    // Basic test for all the input combinations
    task basic_functional_testing();
        $display(" ------------------------------------------------");
        $display(" Basic test ");
        $display(" ------------------------------------------------");

        clk = 1; x = 0; y = 0; c_in = 0;
        #500 clk = 0;
        #500
        for (i = 0; i < TESTSUM; i++) begin
            for (j = 0; j < TESTSUM; j++) begin
                clk = 1;
                x = i[WIDTH-1:0];
                y = j[WIDTH-1:0];
                #500 clk = 0;
                tmp = x + y;
                #500 assert ((s[WIDTH-1:0] == (i[WIDTH-1:0]+j[WIDTH-1:0])) &&
                             (c_out == tmp[WIDTH]))
                     $display("(%0d) x=%d y=%d Cin=%b -> Sum=%d Cout=%b\n",$time,x,y,c_in,s,c_out);
                     else $error("ERROR! (%0d) x=%d y=%d Cin=%b -> Sum=%d Cout=%b\n",$time,x,y,c_in,s,c_out);
            end
        end
    endtask

    // Transition test for evaluating the different delays
    task transition_test(
        input logic [WIDTH-1:0] a0,
        input logic [WIDTH-1:0] b0,
        input logic 			c0,
        input logic [WIDTH-1:0] a1,
        input logic [WIDTH-1:0] b1,
        input logic 			c1);

        $display(" ------------------------------------------------");
        $display(" Transition test: x=%b->%b y=%b->%b Cin=%b->%b",a0,a1,b0,b1,c0,c1);
        $display(" ------------------------------------------------");


        clk = 1; x = a0; y = b0; c_in = c0;
        #500 clk = 0;
        #500 $display("(%0d) FROM: x=%b y=%b Cin=%b -> Sum=%b Cout=%b\n",$time,x,y,c_in,s,c_out);
        clk = 1; x = a1; y = b1; c_in = c1;
        #500 clk = 0;
        #500 $display("(%0d) TO: x=%b y=%b Cin=%b -> Sum=%b Cout=%b\n",$time,x,y,c_in,s,c_out);
    endtask

    task custom_testing(
	input logic [WIDTH-1:0] DNI_u,
	input logic [WIDTH-1:0] DNI_d,
    );

        $display(" ------------------------------------------------");
//        $display(" Custom test: x = %b y = %b" , %DNI_u, %DNI_d);
        $display(" ------------------------------------------------");

	clk = 1; x = DNI_u; y = DNI_d; c_in = 1;
	#500 clk = 0;
        #500 $display("(%0d) TO: x=%b y=%b Cin=%b -> Sum=%b Cout=%b\n",$time,x,y,c_in,s,c_out);
	//clk = 1; x = DNI_u; y = DNI_d; c_in = 1;
	//#500 clk = 0;
	//#500 $display("");
    endtask


    // Main execution flow of the testbench
    initial begin 
        // wavefrom generation
        $dumpfile("waveform.vcd");
        $dumpvars();

        // Functionality testing of the 
        basic_functional_testing();
        transition_test(    
            4'b0000,4'b0000,1'b0,  // from (x,y,c)
            4'b0001,4'b0001,1'b1); // to   (x,y,c)
        custom_testing(
	    4'b1100,4'b0001 //DNI1 = 49456716%16 = 12 / DNI2 = 49456721%16 = 1 
	);
	$finish();
    end
                 
endmodule
