from sklearn import datasets
from sklearn.model_selection import train_test_split
from sklearn.ensemble import GradientBoostingClassifier
from sklearn import metrics

cancer_data = datasets.load_breast_cancer()

X_train, X_test, y_train, y_test = train_test_split(cancer_data.data, cancer_data.target, test_size=0.3, random_state=109) 

classify = GradientBoostingClassifier()

classify.fit(X_train, y_train)

y_pred = classify.predict(X_test)

print("Accuracy:", metrics.accuracy_score(y_test, y_pred))

print("Precision:", metrics.precision_score(y_test, y_pred))

print("Recall:", metrics.recall_score(y_test, y_pred))

print("Confusion Matrix:", metrics.confusion_matrix(y_test, y_pred))

print("Classification Report:", metrics.classification_report(y_test, y_pred))