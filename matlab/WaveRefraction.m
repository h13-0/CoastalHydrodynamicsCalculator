clear all;

% 定义全局变量方便传参到fsolve
global incident_angle k all_points boundary_points Nx Ny dkdx dkdy

% Params: 参数
incident_angle = 0;    % 入射角
depth          = 20;   % 水深
radius         = 2000; % 椭圆半径
tol            = 150;  % 计算间距
cycleTime      = 10;   % 周期
g              = 9.8;  % 重力加速度

% Step1: 找到计算区域内所有内部点
internal_x = [];
internal_y = [];
for x0 = - radius : tol : radius
    if x0 ~= 0
        for y0 = - radius : tol : 0
            if (x0^2 + y0^2 < radius^2)
               internal_x = [internal_x x0];
               internal_y = [internal_y y0];
            end
        end
    end
end
internal_points = length(internal_x);

% Step2: 增加边界点
boundary_x = [];
boundary_y = [];
% 等间距添加x轴边界点
for x0 = - radius : tol : radius
    boundary_x = [boundary_x x0];
    boundary_y = [boundary_y -sqrt(radius^2 - x0^2)];
end
% 等间距添加y轴边界
for y0 = - radius : tol : 0
    boundary_y = [boundary_y y0 y0];
    boundary_x = [boundary_x -sqrt(radius^2 - y0^2) sqrt(radius^2 - y0^2)];
end
% 等间距添加中心边界
for y0 = - radius : tol : 0
    boundary_y = [boundary_y y0];
    boundary_x = [boundary_x 0];
end
boundary_points = length(boundary_x);

% Step3: 求各点水深、波长、波数
all_x = [boundary_x internal_x];
all_y = [boundary_y internal_y];
all_points = length(all_x);
boundary_depth(1: boundary_points) = depth;
internal_depth = sqrt(internal_x.^2 + internal_y.^2) * depth / radius;
all_depth = [boundary_depth internal_depth];
all_wave_length = zeros(1, all_points);
inital_wave_length = 9.8 * cycleTime^2 / 2 / pi;
for i = 1 : 1 : all_points
   f=@(L)L-g.*cycleTime.^2/2/pi*tanh(2*pi*all_depth(i)/L);
   all_wave_length(i)=fzero(f, inital_wave_length);
end
k = 2 * pi ./ all_wave_length;

% Step4: 求k对x和k对y的偏微分
% dL / dh
%dLdh = (1 ./ 2) * cycleTime * sqrt(g ./ all_depth);
dLdh = (9.8 * cycleTime^2./all_wave_length.*(sech(k.*all_depth)).^2)./(1+9.8*cycleTime^2*(sech(k.*all_depth)).^2.*all_depth./all_wave_length.^2);
% dh / dx
dhdx = (all_x .* depth) ./ (radius .* sqrt(all_x.^2 + all_y.^2));
% dk / dx
dkdx = (- 2 * pi ./ all_wave_length .^ 2) .* dLdh .* dhdx;
% dh / dy
dhdy = (all_y .* depth) ./ (radius .* sqrt(all_x.^2 + all_y.^2));
% dk / dy
dkdy = (- 2 * pi ./ all_wave_length .^ 2) .* dLdh .* dhdy;

% Step6: 径向基逼近
% Step6.1: 求解平面空间距离
% Step6.1.1: 求内部点之间的平面空间距离
all_distance = [];      % 各内部点平面距离矩阵
all_distance_max = [];  % 各内部点平面距离最大值
for j = 1 : 1 : all_points
   for n = 1 : 1 : all_points
       all_distance(j, n) = sqrt((all_x(j) - all_x(n))^2 + (all_y(j) - all_y(n))^2);
   end
   all_distance_max(j) = max(all_distance(j, :));
end

% Step6.1.2: 求各点到边界点的平面空间距离
all_boundary_distance = [];      % 各点到边界点的平面距离矩阵
all_boundary_distance_max = [];  % 各点到边界点的平面距离最大值
for j = 1 : 1 : all_points
   for n = 1 : 1 : boundary_points
       all_boundary_distance(j, n) = sqrt((all_x(j) - boundary_x(n))^2 + (all_y(j) - boundary_y(n))^2);
   end
end
all_boundary_distance_max(1 : boundary_points) = all_distance_max(1 : boundary_points);

% Step6.1.3: 求边界点到各点的平面空间距离
boundary_all_distance = [];      % 各边界点到内部点的平面距离矩阵
boundary_all_distance_max = [];  % 各边界点到内部点的平面距离最大值
for j = 1 : 1 : boundary_points
   for n = 1 : 1 : all_points
       boundary_all_distance(j, n) = ((all_x(n) - boundary_x(j))^2 + (all_y(n) - boundary_y(j))^2);
   end
end
boundary_all_distance_max(1 : all_points) = all_distance_max(1 : all_points);

% Step6.1.4: 求边界点之间的平面空间距离
boundary_distance = [];
boundary_distance_max = [];
for j = 1 : 1 : boundary_points
   for n = 1 : 1 : boundary_points
       boundary_distance(j, n) = ((boundary_x(j) - boundary_x(n))^2 + (boundary_y(j) - boundary_y(n))^2);
   end
end
boundary_distance_max(1 : boundary_points) = all_distance_max(1 : boundary_points);

% Step6.2: 计算欧几里得范数和径向基
% 逐行计算内部点之间、内部点和边界点之间的欧几里得范数和径向基
A = [];
for j = 1 : 1 : all_points
	% 计算第j行的欧几里得范数
	all_r = all_distance(j, :) ./ all_distance_max(j);
	all_boundary_r = all_boundary_distance(j, :) ./ all_boundary_distance_max;
	% 计算第j行的径向基
	phi1 = (1 - all_r).^6.*(6 + 36*all_r + 82*all_r.^2 + 72*all_r.^3 + 30*all_r.^4 + 5*all_r.^5);
	phi2 = (1 - all_boundary_r).^4.*(4 + 16*all_boundary_r + 12*all_boundary_r.^2 + 3 * all_boundary_r.^3);
	% 拼接回矩阵phi
    A(j, :) = [phi1 , phi2];
end

% 逐行计算边界点之间、边界点和内部点之间的欧几里得范数和径向基
for j = 1 : 1 : boundary_points
	% 计算第j行的欧几里得范数
	boundary_r = boundary_distance(j, :) ./ boundary_distance_max(j);
	boundary_all_r = boundary_all_distance(j, :) ./ boundary_all_distance_max;
	% 计算第j行的径向基
	phi1 = (21 * boundary_r.^5 - 70 * boundary_r.^3 + 105 * boundary_r - 56).*(boundary_x(j) - boundary_x)./(boundary_distance_max.^2);
	phi2 = (55 * boundary_all_r.^9 - 297*boundary_all_r.^7 + 693*boundary_all_r.^5 - 1155*boundary_all_r.^3 + 792*boundary_all_r.^2 - 88) .* (all_x(j) - all_x)./boundary_all_distance_max.^2;
	% 注意反向
    A(all_points + j, :) = [phi2 , phi1];
end

% Step6.3: 径向基对x、y求偏导
dphidx = [];
dphidy = [];
for j = 1 : 1 : all_points
	dphidx(j, 1 : all_points) = (55 * all_r.^9 - 297 * all_r.^7 + 693 * all_r.^5 - 1155 * all_r.^3 + 792 * all_r.^2 - 88) .* (all_x(j) - all_x) ./ all_distance_max .^ 2;
    dphidx(j, all_points + 1 : all_points + boundary_points) = (21 * all_boundary_r.^5 - 70 * all_boundary_r.^3 + 105 * all_boundary_r - 56) .* (all_x(j) - boundary_x) ./ (all_boundary_distance_max .^ 2);
	dphidy(j, 1 : all_points) = (55 * all_r.^9 - 297 * all_r.^7 + 693 * all_r.^5 - 1155 * all_r.^3 + 792 * all_r.^2 - 88) .* (all_y(j) - all_y) ./ all_distance_max .^ 2;
    dphidy(j, all_points + 1 : all_points + boundary_points) = (21 * all_boundary_r.^5 - 70 * all_boundary_r.^3 + 105 * all_boundary_r - 56) .* (all_y(j) - boundary_y) ./ (all_boundary_distance_max .^ 2);
end

% 逐行计算边界点之间、边界点和内部点之间的欧几里得范数和径向基
for j = 1 : 1 : boundary_points
	dphidx(j + all_points, 1 : all_points) = (55 * boundary_all_r.^9 - 297 * boundary_all_r.^7 + 693 * boundary_all_r.^5 - 1155 * boundary_all_r.^3 + 792 * boundary_all_r.^2 - 88) .* (all_x(j) - all_x) ./ boundary_all_distance_max.^2;
    dphidx(j + all_points, all_points + 1 : all_points + boundary_points) = (21 * boundary_r.^5 - 70 * boundary_r.^3 + 105 * boundary_r - 56) .* (boundary_x(j) - boundary_x) ./ (boundary_distance_max .^ 2);
    dphidy(j + all_points, 1 : all_points) = (55 * boundary_all_r.^9 - 297 * boundary_all_r.^7 + 693 * boundary_all_r.^5 - 1155 * boundary_all_r.^3 + 792 * boundary_all_r.^2 - 88) .* (all_y(j) - all_y) ./ boundary_all_distance_max.^2;
    dphidy(j + all_points, all_points + 1 : all_points + boundary_points) = (21 * boundary_r.^5 - 70 * boundary_r.^3 + 105 * boundary_r - 56) .* (boundary_y(j) - boundary_y) ./ (boundary_distance_max .^ 2);
end

% Step7: 建立方程
% B为A的逆矩阵
B = pinv(A, 4e-14);
Nx = dphidx .* B;
Ny = dphidy .* B;

% Step8: 解方程
% Stpe8.1: 设置alpha初始值
alpha = zeros(1, all_points + boundary_points);
options = optimoptions('fsolve','TolX',1e-6);

% WaveRefractionDiffFunction 为波浪折射方程
alpha = fsolve(@WaveRefractionDiffFunction, alpha, options);
alpha = alpha(1, 1 : all_points);

% Step9: 绘图
% 整点花活
% 绘制等高线
X = - radius : radius * 0.01 : radius;
Y = - radius : radius * 0.01 : 0;
for xi = 1 : 1 : length(X)
   for yi = 1 : 1 : length(Y)
       % 绘制圆锥
       if (X(xi).^2 + Y(yi).^2) <= (radius .^ 2)
           Z(yi, xi) = depth - depth * (sqrt(X(xi).^2 + Y(yi).^2) ./ radius);
       else
           Z(yi, xi) = 0;
       end
   end
end
contourf(X, Y, Z);
hold on;
% 绘制折射图
% 绘制内部点折射图
internal_dx = sin(alpha(boundary_points + 1 : all_points));
internal_dy = cos(alpha(boundary_points + 1 : all_points));
quiver(internal_x, internal_y, internal_dx, internal_dy, 'g', 'Linewidth', 0.5);
% 禁用x、y轴缩放
axis equal
hold on;
% 绘制边界点折射图
boundary_dx = 0.5 * sin(alpha(1 : boundary_points));
boundary_dy = 0.5 * cos(alpha(1 : boundary_points));
quiver(boundary_x, boundary_y, boundary_dx, boundary_dy, 'r', 'Linewidth', 0.5);
hold on;
% 绘制边界
round_alpha = pi : pi / 40 : 2 * pi;
round_x = radius * cos(round_alpha);
round_y = radius * sin(round_alpha);
round_x1 = [- radius : 1 : radius];
round_y1 = zeros(1, length(round_x1));
round_x = [round_x round_x1];
round_y = [round_y round_y1];
plot(round_x, round_y, 'r');
% bingo!
% It's too hard!!!!
