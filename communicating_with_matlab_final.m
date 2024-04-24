%%
clear;
clc;
close all;
%%
device = serialport("COM9",9600);
device.Timeout = 40;

V_initalise = [0 0 0; ...
               0 0 0; ...
               0 0 0];

h = imagesc(V_initalise,[0 10]);
splitArray = ["S1: "," S2: "," S3: ",
              " S4: "," S5: "," S6: ",
              " S7: "," S8: "," S9: "];

%%

while true
    if device.NumBytesAvailable > 0
        data = readline(device);
        newData = split(data,splitArray);
        s1 = str2double(newData(1));
        s2 = str2double(newData(2));
        s3 = str2double(newData(3));
        s4 = str2double(newData(4));
        s5 = str2double(newData(5));
        s6 = str2double(newData(6));
        s7 = str2double(newData(7));
        s8 = str2double(newData(8));
        s9 = str2double(newData(9));

        V = [s1 s2 s3;
             s4 s5 s6;
             s7 s8 s9];

        set(h,'CData', V);
        clim([0, 1000]); % Update the color axis to match sensor range
        drawnow; % Update the figure with new data
        disp(data);
    end
    
    % It may not be necessary to continuously write to the device
    % Depending on the device protocol, you might comment this out.
    write(device,"e","char");
    
    % Introduce a short pause to allow other operations
    pause(0.01); % Pause for 0.01 seconds to allow figure update and not overload with read/write operations
end