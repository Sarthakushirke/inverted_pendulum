clear all, close all, clc

m = 1.5;
l = 0.5;
g = 9.81;

X0 = [pi,0]; % For inverted pendulum X0 = [pi, 0]

A= [0 1; (-g/l)*cos(X0(1)) 0];
B = [0; 1/(m*l^2)];
eig(A)

Poles = [-5; -10 ];

K = place(A,B,Poles); 

eig(A-B*K)

% C = eye(2);
% sys = ss(A,B,C,0*B);

%%

tspan = 0:.001:10;
if(s==-1)
    y0 = [0; 0; 0; 1.5];
    [yL,t,xL] = initial(sys,y0,tspan);
    [t,yNL] = ode45(@(t,y)cartpend(y,m,M,L,g,d,0),tspan,y0);
elseif(s==1)
    y0 = [0; 0; pi+.0001; 0];
    [yL,t,xL] = initial(sys,y0-[0; 0; pi; 0],tspan);
    [t,yNL] = ode45(@(t,y)cartpend(y,m,M,L,g,d,0),tspan,y0);
else    
end

figure
% plot(t,yL);
plot(t,yL+ones(10001,1)*[0; 0; pi; 0]');
hold on
plot(t,yNL);

figure
for k=1:100:length(t)
    drawcartpend_bw(yNL(k,:),m,M,L);
end