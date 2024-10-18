module sumador #( 
    parameter   WIDTH      = 64
)(
    input logic [WIDTH-1:0] 	x_i,
    input logic [WIDTH-1:0]     y_i,
    output logic [WIDTH-1:0]    s_o
);

assign s_o = x_i + y_i;

endmodule;
