clc; clear all; close all; 
d = csvread('lab2-data.csv');

figure;
temp = d(:,1);
volt = d(:,2);

k = polyfit(volt, temp, 2);
fprintf('%0.4f %0.4f %0.4f\n', k(3), k(2), k(1));

x = linspace(min(volt), max(volt), 100); 
temperatureFit = polyval(k, x); 

plot(volt, temp, 'o');
xlabel('Voltage')
ylabel('Temperature')
hold on; 
plot( x, temperatureFit, '-');