
% ������Ϣ
T = 10;  %����
syms h0; %ˮ��
syms k;  %

% ����Ϣ
syms x; %������
syms y; %������
syms h; %���Ӧˮ��
syms L; %���Ӧ����

g = 9.8;    %�������ٶ�
h = sqrt(x.^2 + y.^2) / 100;
L = T*sqrt(g*h);

% k
k = 2*pi/L; 


% ����Ƕ�
syms alpha;
k
diff(k, x)
