% lab2 main
load('YaleFace.mat');
pca_threshold=[0.3,0.6,0.8,0.9,0.92,0.94,0.95,0.96,0.97,0.98,0.99,0.995];
result(1:size(pca_threshold,2),1:size(test_data,1))=0;
for i=1:6
	subplot(6,size(pca_threshold,2)+1,(i-1)*(size(pca_threshold,2)+1)+1),imshow(reshape(test_data(i,:),50,50));
end
for i=1:size(pca_threshold,2)
	[~,recons_data,recons_error]=reconsPCA(train_data,test_data,ground_truth,pca_threshold(1,i));
	result(i,:)=recons_error';
	for j=1:6
		subplot(6,size(pca_threshold,2)+1,(j-1)*(size(pca_threshold,2)+1)+i+1),imshow(reshape(recons_data(j,:),50,50)),title(num2str(pca_threshold(1,i)));
	end
end

[proj_matrix,recons_data,recons_error]=reconsPCA(train_data,test_data,ground_truth,0.95);
for i=1:6
	imwrite(reshape(recons_data(i,:),50,50),cat(2,i+'0','.jpg'));
end
save recons_error.mat recons_error