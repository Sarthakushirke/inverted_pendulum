%% System Parameters
k = 1e4;       % N/m (stiffness)
m = 0.1;       % kg (mass)
b = 0.1;       % Ns/m (damping)
fc = 150;      % Hz, power amplifier cutoff frequency

% Power amplifier as first-order low-pass filter
tau = 1 / (2 * pi * fc); % time constant

x_old = [0,0,0]; % For inverted pendulum X0 = [pi, 0]

A = [0        1         0;
    -k/m    -b/m       1/m;
     0        0    -1/tau];

B = [0; 0; 1/tau];
C_lqr = [1 0 0];     % Output is position
D = 0;

eig(A)

%% 
Poles = [-5; -10; -6 ];

K = place(A,B,Poles); 

output_poles = eig(A-B*K)

%% 
t = 0:0.1:2
u = -K*x_old(t) + 1;
x_new(t) = A*x_old(t) + B*u;
y = C_lqr*x_new(t);

plot(u)




%% Closed-loop system (state feedback)

