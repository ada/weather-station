clc; clear all; close all; 
%d = csvread('lab2-data.csv');
d = csvread('lab2-data-thermistor.csv');
figure;
temp = d(:,1);
volt = d(:,2);

k = polyfit(volt, temp,3);
%fprintf('%0.4f %0.4f\n', k(2), k(1));
fprintf('%0.4f %0.4f %0.4f %0.4f\n', k(4), k(3), k(2), k(1));

x = linspace(min(volt), max(volt), 100); 
temperatureFit = polyval(k, x); 

plot(volt, temp, 'o');
xlabel('Voltage')
ylabel('Temperature')
hold on; 
plot( x, temperatureFit, '-');