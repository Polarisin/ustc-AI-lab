function [ypred,accuracy] = softsvm(traindata, trainlabel, testdata, testlabel , sigma, C)
trainsize=size(traindata,1);
testsize=size(testdata,1);
Kt(1:testsize,1:trainsize)=0;
K(1:trainsize,1:trainsize)=0;
if sigma==0
	K=traindata*traindata';
	Kt=testdata*traindata';
else
	for i=1:trainsize
		K(i,:)=exp(-sum((repmat(traindata(i,:),trainsize,1)-traindata).^2,2)/(sigma^2))';
	end
	for i=1:trainsize
		Kt(:,i)=exp(-sum((repmat(traindata(i,:),testsize,1)-testdata).^2,2)/(sigma^2))';
	end
end
y=trainlabel';
y(y==0)=-1;
opts = optimoptions('quadprog','Algorithm','interior-point-convex');
alpha(1:trainsize,1)=quadprog(K.*(y'*y),ones(1,trainsize)*-1,[],[],y,0,zeros(trainsize,1),ones(trainsize,1)*C,[],opts);
alpha(abs(alpha)<1e-7)=0;
count=sum(alpha~=0);
b=sum(y'-K*(alpha'.*y)')/count;
ypred=Kt*(alpha'.*y)'+b;
ypred(ypred>=0)=1;
ypred(ypred<0)=0;
accuracy=sum(~xor(ypred,testlabel))/testsize;