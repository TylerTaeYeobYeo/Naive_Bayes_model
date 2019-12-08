# Naive Bayes Model

### quick build & use:
```
cd example
make
./trainer
./predictor
```

### to set stopwords

```
cd lib
vim stopwords
```
There is a file under 'lib' folder called 'stopwords'. To add or remove stopwords, eplease edit the contents of the file. To comment a line, please put '#' character at the start of the line.


# 1. trainer
##  a. data
The train data sets should be located in the “data” folder. There should be 2 files named as “train.negative.csv” and “train.non-negative.csv”.
##  b. stopwords
The list of words is saved as an external file. This file should be located at the “lib” folder with the name of “stopwords”. To comment within this file, please place the ‘#’ character at the front of the line without any space.
# 2. predictor
#   a. model.csv
The “model.csv” file should be placed in the same location as the predictor. The location of the predictor will be at “src” folder, when ‘make’ command is used to build.
##  b. data
The test data sets should be located in the “data” folder. There should be 2 files named as “test.negative.csv” and “test.non-negative.csv”.
