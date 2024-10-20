module tb_buffer_circular;

    parameter WIDTH = 64;
    parameter NUM = 8;
    parameter INDEX_SIZE = $clog2(NUM);
    

    reg clk, rstn;
    logic insercion;
    logic [WIDTH-1:0] dato;
    logic delecion;
    logic [WIDTH-1:0] dato_output;
    logic vacia;
    logic llena;
    logic [WIDTH-1:0] i;
    logic [WIDTH-1:0] tmp;
    
    initial begin
        
    end

    // Instance of the full_adder under test
    BufferCircular #( .WIDTH(WIDTH), .NUM(NUM)) hola (
	        .clk_i(clk), 			   // Senyal de reloj
  		.rstn_i(rstn), 		       // Senyal negada de reset
  		.insercion_i(insercion),
  		.dato_i(dato),
		.delecion_i(delecion),
  		.dato_o(dato_output),
  		.vacia_o(vacia),
		.llena_o(llena)
    );

    // Basic test for all the input combinations
    task basic_functional_testing();
        $display(" ------------------------------------------------");
        $display(" Basic test ");
        $display(" ------------------------------------------------");
// LLENAR BUFFER
        clk = 1; rstn = 0;
        #500 clk = 0; rstn = 1; insercion = 0; delecion = 0; dato = 1; tmp = 0;
        #500 assert (vacia)
             $display("(%0d) insercion=%d dato=%d delecion=%d dato_output=%d vacia=%d llena=%d\n",$time,insercion,dato,delecion, dato_output, vacia, llena);
             else $error("ERROR! (%0d) insercion=%d dato=%d delecion=%d dato_output=%d vacia=%d llena=%d\n",$time,insercion,dato,delecion, dato_output, vacia, llena);
        for (i=0; i<8; i++) begin
             clk = 1; tmp = i+1;
             #500 clk = 0; rstn = 1; insercion = 1; delecion = 0; dato = tmp;
             #500 assert (!llena)
		  $display("(%0d) insercion=%d dato=%d delecion=%d dato_output=%d vacia=%d llena=%d\n",$time,insercion,dato,delecion, dato_output, vacia, llena);
		  else $error("ERROR! (%0d) insercion=%d dato=%d delecion=%d dato_output=%d vacia=%d llena=%d\n",$time,insercion,dato,delecion, dato_output, vacia, llena);
        end
// VACIAR BUFFER
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
