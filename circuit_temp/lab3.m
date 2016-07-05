clc; clear all; close all; 
d = csvread('lab3.csv');
figure;
distance2 = linspace (0, 30, 100);
distance = d(:,1);
voltage = d(:,2);
plot(distance, voltage,'-*');
xlabel('distance')
ylabel('voltage')
hold on; 

% Parameter Vector: b(1) = a,  b(2) = b,  b(3) = c
yfit = @(b,x) b(1)./(x.^3 + b(2)) + b(3);  % Objective Function

CF = @(b) sum((voltage-yfit(b,distance)).^2);        % Cost Function

fv = 1;

while fv > 0.01
    b0 = rand(1,3)*10;                      % Initial Parameter Estimates
    [B, fv] = fminsearch(CF, b0)          % Estimate Parameters
end


plot(distance2, yfit(B, distance2))
