% lab1 main
load('usps_3_8.mat');

nbayes_threshold=[0.5,0.6,0.7,0.75,0.8,0.85,0.9];
part=size(digits_data,1)/5;
nbayes_result(size(nbayes_threshold,2),5)=0;
for i=0:4
	traindata=cat(1,digits_data(1:i*part,:),digits_data(i*part+part+1:size(digits_data,1),:));
	trainlabel=cat(1,digits_label(1:i*part,:),digits_label(i*part+part+1:size(digits_label,1),:));
	testdata=digits_data(i*part+1:i*part+part,:);
	testlabel=digits_label(i*part+1:i*part+part,:);
	for j=1:size(nbayes_threshold,2)
		threshold=nbayes_threshold(1,j);
		[ypred,accuracy]= nbayesclassifier (traindata, trainlabel, testdata, testlabel, threshold);
		nbayes_result(j,i+1)=accuracy;
	end
end
[~,index]=max(mean(nbayes_result,2));
threshold=nbayes_threshold(1,index);

ls_lambda=[1e-4,0.01,0.1,0.5,1,5,10,100,1000,5000,10000];
ls_result(size(ls_lambda,2),5)=0;
for i=0:4
	traindata=cat(1,digits_data(1:i*part,:),digits_data(i*part+part+1:size(digits_data,1),:));
	trainlabel=cat(1,digits_label(1:i*part,:),digits_label(i*part+part+1:size(digits_label,1),:));
	testdata=digits_data(i*part+1:i*part+part,:);
	testlabel=digits_label(i*part+1:i*part+part,:);
	for j=1:size(ls_lambda,2)
		lambda=ls_lambda(1,j);
		[ypred,accuracy] = lsclassifier(traindata, trainlabel, testdata, testlabel, lambda);
		ls_result(j,i+1)=accuracy;
	end
end
[~,index]=max(mean(ls_result,2));
lambda=ls_lambda(1,index);

softsvm_C=[1,10,100,1000];
softsvm_sigma(5,5)=0;
softsvm_result(size(softsvm_sigma,2)*size(softsvm_C,2),5)=0;
for i = 0:4
	traindata=cat(1,digits_data(1:i*part,:),digits_data(i*part+part+1:size(digits_data,1),:));
	trainlabel=cat(1,digits_label(1:i*part,:),digits_label(i*part+part+1:size(digits_label,1),:));
	testdata=digits_data(i*part+1:i*part+part,:);
	testlabel=digits_label(i*part+1:i*part+part,:);
	d=0;
	for tempi=1:size(traindata,1)
		d=d+sum(sum((repmat(traindata(tempi,:),size(traindata,1),1)-traindata).^2))/(size(traindata,1)^2);
	end
	softsvm_sigma(i+1,:)=[0.01,0.1,1,10,100].*d;
	for j=1:size(softsvm_sigma,2)
		for k=1:size(softsvm_C,2)
			sigma=softsvm_sigma(i+1,j);
			C=softsvm_C(1,k);
			[ypred,accuracy] = softsvm(traindata, trainlabel, testdata, testlabel , sigma, C);
			softsvm_result((j-1)*size(softsvm_C,2)+k,i+1)=accuracy;
		end
	end
end
[~,index]=max(mean(softsvm_result,2));
sigma=mean(softsvm_sigma(:,ceil(index/size(softsvm_C,2))));
C=softsvm_C(mod(index,size(softsvm_C,2)));
save usps_3_8_parameter.mat threshold lambda sigma C