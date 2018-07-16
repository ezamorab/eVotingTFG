const path = require('path');

// Load ORM
const Sequelize = require('sequelize');


// To use SQLite data base:
//    DATABASE_URL = sqlite:quizzes.sqlite
// To use  Heroku Postgres data base:
//    DATABASE_URL = postgres://user:passwd@host:port/database

const url = process.env.DATABASE_URL || "sqlite:candidates.sqlite";

const sequelize = new Sequelize(url);

// Import the definition of the Quiz Table from quiz.js
sequelize.import(path.join(__dirname, 'candidate'));

// Sessions
sequelize.import(path.join(__dirname,'sessions'));

// Create tables
sequelize.sync()
.then(() => console.log('Data Bases created successfully'))
.catch(error => {
    console.log("Error creating the data base tables:", error);
    process.exit(1);
});


module.exports = sequelize;
