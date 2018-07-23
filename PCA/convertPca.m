%%
% �������off�ļ�����pcaת���������off�ļ�

%%
% ����ת��
function [] = convert(inputName,outputName)
    [point triangle] = readOff(inputName);
    point = pcaProcess(point);
    saveOff(outputName,point,triangle);
end

%%
% �Ե����ݽ���pca�������ش����ĵ�����
function [pdata] = pcaProcess(data)
    d = cov(data);      % ÿһ����һ������,����Э�������
    [E,D] = eig(d);
    [dummy,order] = sort(diag(-D)); %������ֵ��������
    E = E(:,order);
    pdata = E'*data';   % �ռ�ת��
    pdata = pdata';
end

%%
% д��off�����������������д���ļ�
function [] = saveOff(filename,point,triangle)
    fid = fopen(filename,'w');
    fprintf(fid,'%s\r\n','OFF');
    fprintf(fid,'%d %d %d\r\n',size(point,1),size(triangle,1),0);
    for i =1:size(point,1)
        fprintf(fid,'%f %f %f\r\n',point(i,1),point(i,2),point(i,3));
    end
    for i =1:size(triangle,1)
        fprintf(fid,'%d %d %d %d\r\n',3,triangle(i,1),triangle(i,2),triangle(i,3));
    end
    fclose(fid);
end

%%
% ����off�����ص������������
function [point,triangle] = readOff(filename)
    fid = fopen(filename,'r');
    d = fgetl(fid);
    d = fgetl(fid);
    data = sscanf(d,'%d %d %d');
    point =[];
    triangle = [];    
    for i =1:data(1)
        d = fgetl(fid);
        tmp = sscanf(d,'%f %f %f');
        tmp = tmp';
        point = [point;tmp];
    end
    for i = 1:data(2)
        d = fgetl(fid);
        tmp = sscanf(d,'%f %f %f');
        tmp = tmp';
        triangle = [triangle;tmp(2:4)];
    end
    fclose(fid);
end