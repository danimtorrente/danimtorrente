module adder_4bits (
	input logic [3:0] x_i,
	input logic [3:0] y_i,
	input logic c_i,
	output logic [3:0] s_o,
	output logic c_o
);

	logic [4:0] adder;
	genvar i;
	assign adder[0] = c_i;
	assign c_o = adder[4];
generate
	for (i = 0; i < 4; ++i) begin
		full_adder almendras(
			.x_i(x_i[i]),
			.y_i(y_i[i]), 
			.c_i(adder[i]), 
			.s_o(s_o[i]),
			.c_o(adder[i+1]) 
		);
	end
endgenerate
endmodule
