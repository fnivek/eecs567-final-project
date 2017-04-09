classdef ArmLink < handle
    %UNTITLED3 Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        portName
        portStream
        jointAngles
        endEffectorPos
    end
    
    methods
        function obj = ArmLink()
            obj.portName = 'COM3';
            obj.portStream = serial(obj.portName, 'BaudRate', 9600, 'Terminator', '');
            obj.jointAngles = single(zeros(4, 1));
            obj.endEffectorPos = single(zeros(3, 1));
        end
        
        function open(obj)
            set(obj.portStream,'Port',obj.portName);
            set(obj.portStream,'OutputBufferSize',5120);
            set(obj.portStream,'InputBufferSize',5120);
            set(obj.portStream,'Timeout',2);
            fopen(obj.portStream);
            
            % Start the stream
            fprintf(obj.portStream, '*IDN?');
        end
        
        function close(obj)
            nAvailable = get(obj.portStream, 'BytesAvailable');
            
            if nAvailable > 0
                fread(obj.portStream);
            end
            fclose(obj.portStream);
        end
        
        function [data, count] = read(obj)
            [data, count] = fread(obj.portStream);
        end
        
        function parseMessage(obj, msg)
            if (length(msg) < 4)
                return;
            end
            if (msg(1) ~= hex2dec('AA')) || (msg(2) ~= hex2dec('55'))
                return;
            end
            
            msgLen = msg(3);
            msgOp = msg(4);
            
            if length(msg(4:end)) ~= msgLen
                % Not matching length
                return;
            end
            
            switch msgOp
                % Incoming joint angles
                case hex2dec('10')
                    if msgLen ~= 4 * 4 + 1
                        % Expect 4 floats and opcode
                        return;
                    end
                    rawvals = uint8(reshape(msg(5:20), 4, 4));
                    for i = 1:4
                        obj.jointAngles(i) = typecast(rawvals(:,i), 'single');
                    end
                % Incoming end effector position
                case hex2dec('11')
                    if msgLen ~= 3 * 4 + 1
                        % Expect 3 floats and opcode
                        return;
                    end
                    rawvals = uint8(reshape(msg(5:16), 4, 3));
                    for i = 1:3
                        obj.endEffectorPos(i) = typecast(rawvals(:,i), 'single');
                    end
                otherwise
            end
        end
        
        function delete(obj)
            delete(obj.portStream);
        end
    end
    
end

