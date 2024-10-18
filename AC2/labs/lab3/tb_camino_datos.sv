module tb_camino_datos;

    parameter WIDTH = 32;
    parameter REGNUM = 16;
    parameter INDEX_SIZE = $clog2(REGNUM);
    // Signal declaration
    reg clk, rstn;
    reg inicio;
    reg [WIDTH-1:0] tmp; 
    wire fin;
    wire [WIDTH-1:0] data_output;
    wire [INDEX_SIZE-1:0] regid;
    integer i;

    initial begin
        
    end

    // Instance of the full_adder under test
    CaminoDatos #( .WIDTH(WIDTH), .NUMREG(REGNUM)) CD (
        .clk_i(clk), 			   // Senyal de reloj
  		.rstn_i(rstn), 		       // Senyal negada de reset
  		.inicio_i(inicio),
  		.fin_o(fin),
  		.data_o(data_output),
  		.regId_o(regid)
    );

    // Basic test for all the input combinations
    task basic_functional_testing();
        $display(" ------------------------------------------------");
        $display(" Basic test ");
        $display(" ------------------------------------------------");

        clk = 1; rstn = 0; tmp = 'h0;
        #500 clk = 0; rstn = 1; inicio = 1;
        #500 assert ((fin == 1'b0) && (regid == 4'h0) && (data_output == 32'h0))
             $display("(%0d) fin=%b regid=%d data_o=%d\n",$time,fin,regid,data_output);
             else $error("ERROR! (%0d) fin=%b regid=%d data_o=%d\n",$time,fin,regid,data_output);
        for (i=0; i<6; i++) begin
             clk = 1;
             tmp = tmp + i[WIDTH-1:0] + 1;
             #500 clk = 0; inicio = 0;
             #500 assert (fin == (i == 32'h5) && (regid == 4'h0) && (data_output == tmp))
                  $display("(%0d) fin=%b regid=%d data_o=%d\n",$time,fin,regid,data_output);
                  else $error("ERROR! (%0d) fin=%b regid=%d data_o=%d\n",$time,fin,regid,data_output);
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
