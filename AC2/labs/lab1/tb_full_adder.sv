module full_adder_tb;

    // Signal declaration
    reg clk;
    reg x,y,cin;
    wire sum,carry;
    integer i,j,k;

    initial begin
        
    end

    // Instance of the full_adder under test
    full_adder full_adder_inst(
        .x_i(x),
        .y_i(y),
        .c_i(cin),
        .s_o(sum),
        .c_o(carry));

    // Basic test for all the input combinations
    task basic_functional_testing();
        $display(" ------------------------------------------------");
        $display(" Basic test ");
        $display(" ------------------------------------------------");

        clk = 1; x = 0; y = 0; cin = 0;
        #500 clk = 0;
        #500
        for (i=0;i<2;i++) begin
            for (j=0;j<2;j++) begin
                for (k=0;k<2;k++) begin
                    clk = 1; x = i[0]; y = j[0]; cin = k[0];
                    #500 clk = 0;
                    #500 assert ((i+j+k) == (integer'(sum)+2*integer'(carry))) 
                         $display("(%0d) x=%b y=%b Cin=%b -> Sum=%b Cout=%b\n",$time,x,y,cin,sum,carry);
                         else $error("ERROR! (%0d) x=%b y=%b Cin=%b -> Sum=%b Cout=%b\n",$time,x,y,cin,sum,carry);

                end
            end
        end
    endtask

    // Transition test for evaluating the different delays
    task transition_test(
        input logic a0,
        input logic b0,
        input logic c0,
        input logic a1,
        input logic b1,
        input logic c1);

        $display(" ------------------------------------------------");
        $display(" Transition test: x=%b->%b y=%b->%b Cin=%b->%b",a0,a1,b0,b1,c0,c1);
        $display(" ------------------------------------------------");


        clk = 1; x = a0; y = b0; cin = c0;
        #500 clk = 0;
        #500 $display("(%0d) FROM: x=%b y=%b Cin=%b -> Sum=%b Cout=%b\n",$time,x,y,cin,sum,carry);
        clk = 1; x = a1; y = b1; cin = c1;
        #500 clk = 0;
        #500 $display("(%0d) TO: x=%b y=%b Cin=%b -> Sum=%b Cout=%b\n",$time,x,y,cin,sum,carry);
    endtask

    // Main execution flow of the testbench
    initial begin 
        // wavefrom generation
        $dumpfile("waveform.vcd");
        $dumpvars();

        // Functionality testing of the 
        basic_functional_testing();
        transition_test(    
            1'b0,1'b0,1'b0,  // from (x,y,c)
            1'b1,1'b1,1'b1); // to   (x,y,c)
        $finish();
    end
                 
endmodule
