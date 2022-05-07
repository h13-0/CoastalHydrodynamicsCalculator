function F = WaveRefractionDiffFunction(alpha)
    global incident_angle k all_points boundary_points Nx Ny dkdx dkdy
    % 内部点使用波浪控制偏微分方程
    for i=1:1:all_points
        F(i)=dkdx(i)*sin(alpha(i))+k(i)*cos(alpha(i))*(Nx(i,:)*alpha')-dkdy(i)*cos(alpha(i))+k(i)*sin(alpha(i))*(Ny(i,:)*alpha');
    end
    % 边界点带入波浪控制方程在边界点的特殊形式的偏微分方程
    for i=all_points+1:1:all_points+boundary_points  
        F(i)=alpha(i)-dkdy(i-all_points)/k(i-all_points);
    end
    
    % 设置边界点波浪弧度为-m;
    F(1 : boundary_points) = alpha(1 : boundary_points) - incident_angle;
end