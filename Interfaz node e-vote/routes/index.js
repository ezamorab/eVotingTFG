const express = require('express');
const router = express.Router();

const quizController = require('../controllers/quiz');

/* GET home page. */
router.get('/', (req, res, next) => {
  res.render('index');
});

// Author page.
router.get('/author', (req, res, next) => {
    res.render('author');
});


// Autoload for routes using :quizId
router.param('quizId', quizController.load);


// Routes for the resource /quizzes
router.get('/candidates',                     quizController.index);
router.get('/vote',                     quizController.selectCandidate);
router.get('/validateVote',                     quizController.validateVote);

module.exports = router;
