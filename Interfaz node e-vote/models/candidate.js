
// Definition of the Quiz model:

module.exports = function (sequelize, DataTypes) {
    return sequelize.define('candidate',
        {
            name: {
                type: DataTypes.STRING,
                validate: {notEmpty: {msg: "Question must not be empty"}}
            },
            value: {
                type: DataTypes.STRING,
                validate: {notEmpty: {msg: "Answer must not be empty"}}
            }
        });
};
