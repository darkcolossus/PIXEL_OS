Comandos en octave:

- nombre_variable = dlmread('output.txt');  //nombre_variable puede ser: GPC, GPCV2, BEEMAN, VERLET
- plot(GPC(:,1),GPC(:,2),'ro', GPCV2(:,1),GPCV2(:,2),'bx',BEEMAN(:,1),BEEMAN(:,2),'g^', VERLET(:,1),VERLET(:,2),'m*');