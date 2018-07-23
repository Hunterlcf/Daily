function [] = ICPprocess()
    %%
    %Ѱ�ҵı任��ϵdata2=Rdata1+T
    %data2���䣬data1���ϵĽ���
    %��������ѡȡ���Ƶ�
    d1 = importPoint('data/cube_4_sp.txt');
    d2 = importPoint('data/cube_5_without_points_sp.txt');

    %[d1,d2] = generateData();
    data1 =d1;
    data2 =d2;

    figure(1);
    plot3(data1(:,1),data1(:,2),data1(:,3),'r.');
    hold on;
    plot3(data2(:,1),data2(:,2),data2(:,3),'b.');
    title('ԭʼ����');
    axis tight equal;
    hold off;

    [m,n]=size(data2);
    controldata1=d1;    % ѡȡ���Ƶ�
    controldatanum=size(controldata1,1);
    controldata2=zeros(controldatanum,3); %�����洢�ҵ��Ķ�Ӧ��

    %%
    %��ʼ��
    R=[1,0,0;0,1,0;0,0,1];
    T=[0,0,0];
    last_E=0; %�ϴεĴ�����
    iteration=50; %��������
    R_Intermediate=zeros(3,3,iteration);    %�洢ÿһ�ε�����R����
    T_Intermediate=zeros(3,1,iteration);    %�洢ÿһ�ε�����T����
    delta_Intermediate=zeros(iteration,1);  %�洢ÿһ�ε�����delta
    e_Intermediate=zeros(iteration,1);      %�洢ÿһ�ε�����e
    index=zeros(controldatanum,1);          %�����洢��Ӧ�������

    %%
    %����
    for iter=1:iteration
        %Ѱ�ҿ��Ƶ�Ķ�Ӧ�㣬controldata2�����洢��Ӧ������꣬index�洢��Ӧ�������
        for i=1:controldatanum
            temp_data1=repmat(controldata1(i,:),m,1); %����һ�����ݽ��и��ƣ��������ֱ�ӽ���ʸ����������
            diff=sqrt(sum((temp_data1-data2).^2,2));  %���ÿһ�����ݵ��õ�ľ���,sum(,2)�������
            [minvalue,index(i,1)]=min(diff);          %�ҵ�������С�ģ����õ�����index
            controldata2(i,:)=data2(index(i,1),:);    %����Ӧ�����ݴ洢�ڶ�Ӧ��λ��
        end
        %%
        %����ȷ���Ĺ�ϵ�����RT������controldata1�ǿ��Ƶ�����꣬controldata2���󵽿��Ƶ���������
        %ƥ������������
        centroid1=mean(controldata1);    
        centroid2=mean(controldata2);
        %��ȥ�������� 
        demeancontroldata1=controldata1-repmat(centroid1,controldatanum,1);
        demeancontroldata2=controldata2-repmat(centroid2,controldatanum,1);
        %���H����
        H=demeancontroldata1'*demeancontroldata2;
        [U,S,V]=svd(H);
        R=V*U';                         %���R����S������ֵ
        T=(centroid2-centroid1)';       %���T����

        R_Intermediate(:,:,iter)=R;     %��¼��ÿһ�ε�����RT
        T_Intermediate(:,:,iter)=T;
        %%
        %�������õ���RT����任֮��ĵ�
        controldata1=R*controldata1'+repmat(T,1,controldatanum);
        controldata1=controldata1';                 %�µĿ��Ƶ�
        E=norm(controldata1-controldata2,2);        %���㷶ʽ����
        e_Intermediate(iter,1)=E/controldatanum;    %�������
        delta=abs(E-last_E)/controldatanum;          %�м���������
        delta_Intermediate(iter,1)=delta;   
        if(delta<0.001)
            break;
        end
        last_E=E;           % ������һ�ε����
    end

    %% 
    %��ʾ��������е����
    figure(2);
    plot(1:iter,delta_Intermediate(1:iter,1)');
    xlabel('��������');ylabel('delta');
    figure(3);
    plot(1:iter,e_Intermediate(1:iter)');
    xlabel('��������');ylabel('loss');
    %%
    %�������յ�R��T
    temp_R=eye(3);
    temp_T=zeros(3,1);
    %������õ���RT���кϲ����㣬�õ����յ�RT
    for i=1:iter
       temp_R=R_Intermediate(:,:,i)*temp_R; 
       temp_T=R_Intermediate(:,:,i)*temp_T+T_Intermediate(:,:,i);
    end
    R_final=temp_R;
    T_final=temp_T;
    %���㾭�����������ı任�������
    data1_transformed=R_final*data1'+repmat(T_final,1,size(data1,1));
    data1_transformed=data1_transformed';

    %%
    %��ʾ���ս��
    figure(4);
    plot3(data1_transformed(:,1),data1_transformed(:,2),data1_transformed(:,3),'r.');
    hold on;
    plot3(data2(:,1),data2(:,2),data2(:,3),'b.');
    title('ICP results');
    hold on;
    %������
    for i =1:controldatanum
        data3 = [data2(index(i,1),:);data1_transformed(i,:)];        
        plot3(data3(:,1),data3(:,2),data3(:,3),'-');
    end
    axis equal tight;
    hold off;
    %��ȫ���ĵ���б任       
    
    ad1 = importPointSingle('data/cube_4_allpoints.txt');           %��Щ�����Ǿ�����һ�������
    ad2 = importPointSingle('data/cube_5_without_points_allpoints.txt');
    data1_transformed=R_final*ad1'+repmat(T_final,1,size(ad1,1));
    data1_transformed=data1_transformed';
    
    index = zeros(size(ad2,1),1);
    controldata2 = zeros(size(ad2));
                
    figure(5);
    plot3(data1_transformed(:,1),data1_transformed(:,2),data1_transformed(:,3),'r.');
    hold on;
    plot3(ad2(:,1),ad2(:,2),ad2(:,3),'b.');
    hold on;
    
    %Ѱ��ȫ�ֵĶ�Ӧ��
    for i=1:size(ad1,1)
        temp_data1=repmat(ad2(i,:),size(data1_transformed,1),1); %����һ�����ݽ��и��ƣ��������ֱ�ӽ���ʸ����������
        diff=sqrt(sum((temp_data1-data1_transformed).^2,2));  %���ÿһ�����ݵ��õ�ľ���,sum(,2)�������
        [minvalue,index(i,1)]=min(diff);          %�ҵ�������С�ģ����õ�����index        
        data = [data1_transformed(index(i,1),:);ad2(i,:)];
        plot3(data(:,1),data(:,2),data(:,3),'-');
    end  
    
    % �洢���Ӧ������
    fid = fopen('data/cube_4_index.txt','w');
    fprintf(fid,'%d\r\n',size(index,1));
    for i=1:size(index,1)
        fprintf(fid,'%d\r\n',index(i,:));
    end
    fclose(fid);
    
    title('��1���б任')
    hold off;
    
    data1_transformed=R_final'*ad2'-repmat(T_final,1,size(ad2,1));
    data1_transformed=data1_transformed';
    figure(6);
    plot3(data1_transformed(:,1),data1_transformed(:,2),data1_transformed(:,3),'r.');
    hold on;
    plot3(ad1(:,1),ad1(:,2),ad1(:,3),'b.');
    title('��2���б任')
    hold off;
end

%% 
%��������
function [d1,d2] = generateData()
    size1 =[3,4,3];     % ���õĳ�����ĳ����
    size2 =[3,3,3];
    sita1 =30/180*pi;
    Tr = [1.2,2.8,1.2];
    rotatoMat =[cos(sita1),0,-sin(sita1);
        0,1,0;
        sin(sita1),0,cos(sita1)];
    d1=[];
    d2=[];
    % ���ɵ�һ�����������������
    for i=1:100
        % ����������
        t1 = [size1(1),rand()*size1(2),rand()*size1(3)]; 
        d1 = [d1;t1];
        t1 = [0,rand()*size1(2),rand()*size1(3)];
        d1 = [d1;t1];
        % ����������
        t1 = [rand()*size1(1),size1(2),rand()*size1(3)];
        d1 = [d1;t1];
        t1 = [rand()*size1(1),0,rand()*size1(3)];
        d1 = [d1;t1];
        % ǰ��������
        t1 = [rand()*size1(1),rand()*size1(2),size1(3)];
        d1 = [d1;t1];
        t1 = [rand()*size1(1),rand()*size1(2),0];
        d1 = [d1;t1];
    end 
    % ���ɵڶ������������������
    for i=1:100
        t1 = [size2(1),rand()*size2(2),rand()*size2(3)];
        d2 = [d2;t1];
        t1 = [0,rand()*size2(2),rand()*size2(3)];
        d2 = [d2;t1];
        t1 = [rand()*size2(1),size2(2),rand()*size2(3)];
        d2 = [d2;t1];
        t1 = [rand()*size2(1),0,rand()*size2(3)];
        d2 = [d2;t1];
        t1 = [rand()*size2(1),rand()*size2(2),size2(3)];
        d2 = [d2;t1];
        t1 = [rand()*size2(1),rand()*size2(2),0];
        d2 = [d2;t1];
    end 
    %d1 = (rotatoMat*d1')+repmat(Tr',1,size(d1,1));
    dt = d1;
    d1 = d1'+repmat(Tr',1,size(d1,1));      % ��ԭ�������ݽ���ƫ�ƣ�ƫ��tr�ķ���
    d1 = d1';
end

%%
% ����ģ��
function [data] = importPoint(filename)
    data =[];
    fid = fopen(filename,'r');
    str = fgetl(fid);
    cnt = sscanf(str,'%d');
    for i=1:cnt
        str = fgetl(fid);
        tmp = sscanf(str,'%f %f %f');
        data = [data;tmp'];
        % ��ȡ����
        str = fgetl(fid);
        str = fgetl(fid);
    end
    fclose(fid);
end

function [data] = importPointSingle(filename)
    data =[];
    fid = fopen(filename,'r');
    str = fgetl(fid);
    cnt = sscanf(str,'%d');
    for i=1:cnt
        str = fgetl(fid);
        tmp = sscanf(str,'%f %f %f');
        data = [data;tmp'];
    end
    fclose(fid);
end