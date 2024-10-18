module bancoRegistros #(
    parameter   WIDTH      = 64,
    parameter   NUM        = 8,
    localparam  INDEX_SIZE = $clog2(NUM)

)(
input logic                   clk_i,  // Senyal de reloj
input logic                   rstn_i, // Senyal negada de reset
input logic [INDEX_SIZE-1:0]  A_i,    // ID Registro Lectura 1
input logic [INDEX_SIZE-1:0]  B_i,    // ID Registro Lectura 2
input logic                   we_i,   // Permiso de escritura
input logic [WIDTH-1:0]       dato_i, // Dato a escribir
input logic [INDEX_SIZE-1:0]  wid_i,  // ID registro escritura

output logic [WIDTH-1:0]      RegA_o, // Registro A
output logic [WIDTH-1:0]      RegB_o  // Registro B
);

// Instanciamos los registros WIDTHxNUM
logic [WIDTH-1:0]  registers_q  [NUM-1:0];

// Logica de Lectura
always_comb begin
    RegA_o = registers_q[A_i];
    RegB_o = registers_q[B_i];
end

// Logica de Escritura
always_ff @(posedge clk_i, rstn_i) begin
    if (!rstn_i) begin // Reset
        for(integer i=0; i<NUM; ++i) begin
            registers_q[i] <= i[WIDTH-1:0];
        end
    end else if (we_i) begin 
        registers_q[wid_i] <=  dato_i;
    end
end

endmodule
