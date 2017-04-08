function PlotArm(axesHandle, q)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

nJoints = 3;
a = [6, 6, 6];
d = [1.0, 1.5, -1.5];
axisDrawScale = 3.0;

dhtable = [[q(1),        d(1),   0,      -pi/2,  1];...
           [q(2)-pi/2,   d(2),   a(2),   0,      1];...
           [q(3),        d(3),   a(3),   0,      1]];

arm = Manipulator(dhtable);

origins = zeros(3, 3);
xdirs = zeros(3, 3);
ydirs = zeros(3, 3);
zdirs = zeros(3, 3);
for q = 1:nJoints
     res = arm.GetTransform(0, q) * [0; 0; 0; 1];
     origins(:,q) = res(1:3, 1);
     res = arm.GetTransform(0, q) * [1; 0; 0; 0];
     xdirs(:,q) = res(1:3, 1);
     res = arm.GetTransform(0, q) * [0; 1; 0; 0];
     ydirs(:,q) = res(1:3, 1);
     res = arm.GetTransform(0, q) * [0; 0; 1; 0];
     zdirs(:,q) = res(1:3, 1);
end

% Actual plotting
% Wipe from last time
hold(axesHandle, 'off');
% Draw origins
scatter3(axesHandle, origins(1,:), origins(2,:), origins(3,:), 'ko', 'filled');
hold(axesHandle, 'on');
% Draw joint axes
for q = 1:nJoints
    comb = repmat(origins(:,q), 1, 4);
    comb = comb + [zeros(3,1), axisDrawScale * xdirs(:,q), axisDrawScale * ydirs(:,q), axisDrawScale * zdirs(:,q)];
    plot3(axesHandle, comb(1,[1,2]), comb(2,[1,2]), comb(3,[1,2]), ':r', 'LineWidth', 2);
    plot3(axesHandle, comb(1,[1,3]), comb(2,[1,3]), comb(3,[1,3]), ':g', 'LineWidth', 2);
    plot3(axesHandle, comb(1,[1,4]), comb(2,[1,4]), comb(3,[1,4]), ':b', 'LineWidth', 2);
end
% Draw arms
for q = [2, 3]
    comb = [origins(:,q), origins(:,q) - a(q - 1) * xdirs(:,q)];
    plot3(axesHandle, comb(1,:), comb(2,:), comb(3,:), 'Color', [0.5,0.5,0.5], 'LineWidth', 4);
end
% Set view
axis(axesHandle, [-12, 12, -12, 12, 0, 15]);
grid(axesHandle, 'off');
view(axesHandle, 0, 45);


end

