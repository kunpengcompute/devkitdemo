# https://www.kaggle.com/code/simulacra/titanic-with-xgboost
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import RandomizedSearchCV
from xgboost import XGBClassifier


def process_data(train, test):
    X_full = pd.concat([train.drop('Survived', axis = 1), test], axis = 0)
    X_full.drop('PassengerId', axis = 1, inplace=True)

    print(X_full.isnull().sum())
    print((X_full.Age.isnull() & X_full.Cabin.isnull()).sum())
    print(train.Survived.mean())
    print(train.Cabin.notnull().mean())
    print((train.Cabin.isnull() & (train.Survived == 0)).mean())

    selector = (train.Cabin.isnull() & train.Age.isnull())
    print(train[selector].Survived.mean())
    print(train.Survived.mean())

    selector = (train.Cabin.isnull())
    print(train[selector].Survived.mean())

    X_full['Nulls'] = X_full.Cabin.isnull().astype('int') + X_full.Age.isnull().astype('int')
    X_full['Cabin_mapped'] = X_full['Cabin'].astype(str).str[0] # this captures the letter

    # this transforms the letters into numbers
    cabin_dict = {k:i for i, k in enumerate(X_full.Cabin_mapped.unique())} 
    X_full.loc[:, 'Cabin_mapped'] = X_full.loc[:, 'Cabin_mapped'].map(cabin_dict)

    print(cabin_dict)
    print(X_full.columns)

    X_full.drop(['Age', 'Cabin'], inplace = True, axis = 1)
    fare_mean = X_full[X_full.Pclass == 3].Fare.mean()
    X_full['Fare'].fillna(fare_mean, inplace = True)
    print(X_full.isnull().sum())
    print(X_full[X_full.Embarked.isnull()])
    print(X_full[X_full['Pclass'] == 1].Embarked.value_counts())

    X_full['Embarked'].fillna('S', inplace = True)
    print(X_full.isnull().sum())

    X_full.drop(['Name', 'Ticket'], axis = 1, inplace = True)
    X_dummies = pd.get_dummies(X_full, columns = ['Sex', 'Nulls', 'Cabin_mapped', 'Embarked'], drop_first= True)
    return X_dummies


def main():
    train = pd.read_csv('./input/train.csv')
    test = pd.read_csv('./input/test.csv')
    X_dummies = process_data(train, test)

    X = X_dummies[:len(train)]
    new_X = X_dummies[len(train):]
    y = train.Survived

    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size = .3, random_state = 5, stratify = y)

    rf = RandomForestClassifier()
    rf.fit(X_train, y_train)
    print("RandomForestClassifier score: ", rf.score(X_test, y_test))

    xgb = XGBClassifier()
    xgb.fit(X_train, y_train)
    print("XGBClassifier score: ", xgb.score(X_test, y_test))

    lg = LogisticRegression()
    lg.fit(X_train, y_train)
    print("LogisticRegression score: ", lg.score(X_test, y_test))

    # Create the parameter grid: gbm_param_grid 
    gbm_param_grid = {
        'n_estimators': range(8, 20),
        'max_depth': range(6, 10),
        'learning_rate': [.4, .45, .5, .55, .6],
        'colsample_bytree': [.6, .7, .8, .9, 1]
    }

    # Instantiate the regressor: gbm
    gbm = XGBClassifier(n_estimators=10)

    # Perform random search: grid_mse
    xgb_random = RandomizedSearchCV(param_distributions=gbm_param_grid, 
                                    estimator = gbm, scoring = "accuracy", 
                                    verbose = 1, n_iter = 50, cv = 4)

    # Fit randomized_mse to the data
    xgb_random.fit(X, y)

    # Print the best parameters and lowest RMSE
    print("Best parameters found: ", xgb_random.best_params_)
    print("Best accuracy found: ", xgb_random.best_score_)

    xgb_pred = xgb_random.predict(new_X)
    submission = pd.concat([test.PassengerId, pd.DataFrame(xgb_pred)], axis = 'columns')
    submission.columns = ["PassengerId", "Survived"]
    submission.to_csv('titanic_submission.csv', header = True, index = False)


if __name__ == "__main__":
    main()
