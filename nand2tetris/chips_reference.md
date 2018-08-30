* And { **IN** a, b;  **OUT** out; }
* And16 { **IN** a[16], b[16]; **OUT** out[16]; }
* DMux { **IN** in, sel; **OUT** a, b; }
* DMux4Way { **IN** in, sel[2]; **OUT** a, b, c, d; }
* DMux8Way { **IN** in, sel[3]; **OUT** a, b, c, d, e, f, g, h; }
* Mux { **IN** a, b, sel; **OUT** out; }
* Mux4Way16 { **IN** a[16], b[16], c[16], d[16], sel[2]; **OUT** out[16]; }
