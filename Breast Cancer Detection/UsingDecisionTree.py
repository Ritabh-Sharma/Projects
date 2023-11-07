# Importing essential libraries
from sklearn import datasets
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn import metrics

# Load the dataset
cancer_data = datasets.load_breast_cancer()

# Split the data into training/testing sets
X_train, X_test, y_train, y_test = train_test_split(cancer_data.data, cancer_data.target, test_size=0.3, random_state=109) # 70% training and 30% test

# Generating Model
classify = DecisionTreeClassifier()

# Train the model using the training sets
classify.fit(X_train, y_train)

# Predict the response for test dataset
y_pred = classify.predict(X_test)

# Evaluating the Model
# Model Accuracy: how often is the classifier correct?
print("Accuracy:", metrics.accuracy_score(y_test, y_pred))

# Model Precision: what percentage of positive tuples are labeled as such?
print("Precision:", metrics.precision_score(y_test, y_pred))

# Model Recall: what percentage of positive tuples are labelled as such?
print("Recall:", metrics.recall_score(y_test, y_pred))

print("Confusion Matrix:", metrics.confusion_matrix(y_test, y_pred))
print("Classification Report:", metrics.classification_report(y_test, y_pred))
