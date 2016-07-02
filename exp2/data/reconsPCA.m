function [proj_matrix,recons_data,recons_error]=reconsPCA(train_data, test_data, ground_truth, threshold)
[trainsize,datawidth]=size(train_data);
X=train_data;
C=(X'*X)/(trainsize-1);
[V,D]=eig(C);
[~,order]=sort(diag(-D));
V=V(:,order);
d=diag(D);
newd=d(order);
sumd=sum(newd);
for j=1:length(newd)
	i=sum(newd(1:j,1))/sumd;
	if i>threshold
		cols=j;
		break;
	end
end
proj_matrix=V(:,1:cols);
newX=test_data*proj_matrix;
recons_data=newX*proj_matrix';
recons_error=sum(abs(ground_truth-recons_data),2)/(datawidth^2);
end