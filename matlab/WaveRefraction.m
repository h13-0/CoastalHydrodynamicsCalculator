
% 波浪信息
T = 10;  %周期
syms h0; %水深
syms k;  %

% 点信息
syms x; %横坐标
syms y; %纵坐标
syms h; %点对应水深
syms L; %点对应波长

g = 9.8;    %重力加速度
h = sqrt(x.^2 + y.^2) / 100;
L = T*sqrt(g*h);

% k
k = 2*pi/L; 


% 输出角度
syms alpha;
k
diff(k, x)
