* Not { **IN** in; **OUT** out; }
* Not16 { **IN** in[16]; **OUT** out[16]; }
* And { **IN** a, b;  **OUT** out; }
* And16 { **IN** a[16], b[16]; **OUT** out[16]; }
* Or { **IN** a, b; **OUT** out; }
* Or16 { **IN** a[16], b[16]; **OUT** out[16]; }
* Or8Way { **IN** in[8]; **OUT** out;
* Xor { **IN** a, b; **OUT** out; }
* DMux { **IN** in, sel; **OUT** a, b; }
* DMux4Way { **IN** in, sel[2]; **OUT** a, b, c, d; }
* DMux8Way { **IN** in, sel[3]; **OUT** a, b, c, d, e, f, g, h; }
* Mux { **IN** a, b, sel; **OUT** out; }
* Mux4Way16 { **IN** a[16], b[16], c[16], d[16], sel[2]; **OUT** out[16]; }
* Mux8Way16 { **IN** a[16], b[16], c[16], d[16], e[16], f[16], g[16], h[16], sel[3]; **OUT** out[16];
* Mux16 { **IN** a[16], b[16], sel; **OUT** out[16]; }

* HalfAdder { **IN** a, b; **OUT** sum, carry; }
* FullAdder { **IN** a, b, c; **OUT** sum, carry; }
* Add16 { **IN** a[16], b[16]; **OUT** out[16]; }
* Inc16 { **IN** in[16]; **OUT** out[16]; }
* ALU { **IN** x[16], y[16], zx, nx, zy, ny, f, no; **OUT** out[16], zr, ng; }
