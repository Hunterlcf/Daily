%%
% ����dft
function [] = testDFT()
    N =8;
    n = [0:1:N-1];
    xn = 0.5.^n;        % ָ���ź�
    
    I=imread('lenna.jpg');
    I=rgb2gray(I);
    I=im2double(I);
    figure(8);
    imshow(I);
    
    %imagefft();
    imageDFT2fft();
    %imageDFT2();
    %imageDFT();
    %DFTusefft(xn,n,N);  % ʹ��fftʵ��
    %DFT(xn,n,N);
    %testDTFT(xn,n,N);
end

%%
% ͼ��Ķ�άDFT
% ��DFT���зֽ⣬�Ȱ��н���FFT��Ȼ����FFT��ʹ�����õ�fft����dft��
function [] = imageDFT2fft()
    I=imread('lenna.jpg');
    I=rgb2gray(I);
    I=im2double(I);
    [x,y] = size(I);
    Ax = ones(x,y);
    ans = ones(x,y);
    com = 0+1i;
    % ��ÿһ�н���DFT  
    for m=1:y
        Ax(:,m) = fft(I(:,m));
    end
    % ��ÿһ�н���DFT    
    for k=1:x
        ans(k,:) = fft(Ax(k,:));
    end
    F=fftshift(ans);
    F= abs(F);
    F=log(F+1);
    figure(7);
    imshow(F,[]);
end

%%
% ͼ��Ķ�άDFT
% ��DFT���зֽ⣬�Ȱ��н���DFT��Ȼ����DFT
function [] = imageDFT2()
    I=imread('lenna.jpg');
    I=rgb2gray(I);
    I=im2double(I);
    [x,y] = size(I);
    Ax = ones(x,y);
    ans = ones(x,y);
    com = 0+1i;
    % ��ÿһ�н���DFT
    for k =1:x        
        for m=1:y
            sn =0;
            for n =1:x
                sn =sn + I(n,m)*exp(-com*2*pi*k*n/x);
            end
            Ax(k,m) = sn;
        end
    end
    % ��ÿһ�н���DFT
    for l =1:y
        for k =1:x
            sn =0;
            for m=1:y
                sn = sn+Ax(k,m)*exp(-com*2*pi*l*m/y);
            end
            ans(k,l) = sn;
        end
    end    
    F=fftshift(ans);
    F= abs(F);
    F=log(F+1);
    figure(6);
    imshow(F,[]);
end

%%
% ͼ��Ķ�άDFT
% ֱ�ӽ���ԭʼ�Ķ�άDFT���Ĳ�ѭ��
function [] = imageDFT()
    I=imread('lenna_s.jpg');
    I=rgb2gray(I);
    I=im2double(I);
    [x,y] = size(I);
    ans = ones(x,y);
    com = 0+1i;
    for u =1:x
        for v= 1:y
            sn =0;
            for i=1:x                
                for j=1:y
                    sn = sn+I(i,j)*exp(-com*2*pi*(u*i/x+v*j/y));
                end
            end
            ans(u,v) = sn;
        end
    end
    F=fftshift(ans);
    F= abs(F);
    F=log(F+1);
    figure(5);
    imshow(F,[]);
end

%%
% ͼ��Ķ�άDFT
% ʹ�����õ�fft2
function [] = imagefft()
    I=imread('lenna.jpg');
    I=rgb2gray(I);
    I=im2double(I);
    F=fft2(I);
    F=fftshift(F);
    F=abs(F);
    T=log(F+1);
    figure(4);
    imshow(T,[]);
end

%%
% һάDFT
function [] = DFT(xn,n,N)
    Xk = zeros(1,N);    
    for k=1:N
        sn =0.0;
        for i=1:N
            sn = sn+xn(i)*exp(-j*2*pi*i*k/N);
        end
        Xk(k) = sn;
    end
    figure(2);
    %Xk=fft(xn,N);       % ����Ҷ�任
    subplot(211);
    stem(n,xn);
    title('ԭ�ź�');

    subplot(212);
    stem(n,abs(Xk));
    title('DFT')
end

%%
% һάfft
function [] = DFTusefft(xn,n,N)
    figure(1);
    Xk=fft(xn,N);       % ����Ҷ�任
    subplot(211);
    stem(n,xn);
    title('ԭ�ź�');

    subplot(212);
    stem(n,abs(Xk));
    title('FFT�任')
end

%% 
% һάdtft
function [] = testDTFT(xn,n,N)
    figure(3);
    w=[-800:1:800]*4*pi/800;     %Ƶ��-800----+800 �ĳ��ȣ���Ӧ�������Ƶ�������٣���ʡȥ��    
    w = [-N/2:1:N/2]*4*pi*2/N;
    X=xn*exp(-j*(n'*w));         %��dtft�任������ԭʼ����ķ������Ը�ָ��������Ͷ���
    subplot(211)
    stem(n,xn);
    title('ԭʼ�ź�(ָ���ź�)');
    subplot(212);
    plot(w/pi,abs(X));
    title('DTFT�任')
end

