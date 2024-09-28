module full_adder (
	input logic x_i,
	input logic y_i,
	input logic c_i,

	output logic s_o,
	output logic c_o
);

// Declaracion de señales intermedias
	logic A;
	logic B;
	logic D;

// Arquitectura de un full adder
	assign #(100) A = x_i ^ y_i;
	assign #(50) B = A & c_i;
	assign #(50) D = x_i & y_i;
	assign #(100) c_o = B | D;
	assign #(100) s_o = A ^ c_i;

endmodule
