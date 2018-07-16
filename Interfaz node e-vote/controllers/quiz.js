const sequelize = require("sequelize");
const {models} = require("../models");
const operator = sequelize.Op;
const fs = require('fs');
var exec = require('child_process').exec;
// Autoload the quiz with id equals to :quizId
exports.load = (req, res, next, quizId) => {

    models.quiz.findById(quizId)
        .then(quiz => {
            if (quiz) {
                req.quiz = quiz;
                next();
            } else {
                throw new Error('There is no quiz with id=' + quizId);
            }
        })
        .catch(error => next(error));
};

// GET /vote
exports.selectCandidate = (req, res, next) => {
	const valor = req.query.candidatoValue;
	const candidato = req.query.candidatoName;

	// Aquí tengo que crear el fichero
	fs.writeFile("./test.txt", valor, function (err) {
		if (err) {
        	return console.log(err);
    		}

	});


    	res.render('showCandidate', {candidato});
};

// GET /validateVote
exports.validateVote = (req, res, next) => {
	
	  exec('./HELib/keyGenerator', function (err, stdout, stderr){
		console.log("\n Generando voto\n");
	  });
    	res.render('validateVote');
};

// GET /quizzes
exports.index = (req, res, next) => {

    models.candidate.findAll()
        .then(candidates => {
            res.render('candidates', {candidates});
        })
        .catch(error => next(error));
};


// GET /quizzes/:quizId
exports.show = (req, res, next) => {

    const {quiz} = req;

    res.render('quizzes/show', {quiz});
};


// GET /quizzes/new
exports.new = (req, res, next) => {

    const quiz = {
        question: "",
        answer: ""
    };

    res.render('quizzes/new', {quiz});
};

// POST /quizzes/create
exports.create = (req, res, next) => {

    const {question, answer} = req.body;

    const quiz = models.quiz.build({
        question,
        answer
    });

    // Saves only the fields question and answer into the DDBB
    quiz.save({fields: ["question", "answer"]})
        .then(quiz => {
            req.flash('success', 'Quiz created successfully.');
            res.redirect('/quizzes/' + quiz.id);
        })
        .catch(sequelize.ValidationError, error => {
            req.flash('error', 'There are errors in the form:');
            error.errors.forEach(({message}) => req.flash('error', message));
            res.render('quizzes/new', {quiz});
        })
        .catch(error => {
            req.flash('error', 'Error creating a new Quiz: ' + error.message);
            next(error);
        });
};


// GET /quizzes/:quizId/edit
exports.edit = (req, res, next) => {

    const {quiz} = req;

    res.render('quizzes/edit', {quiz});
};


// PUT /quizzes/:quizId
exports.update = (req, res, next) => {

    const {quiz, body} = req;

    quiz.question = body.question;
    quiz.answer = body.answer;

    quiz.save({fields: ["question", "answer"]})
        .then(quiz => {
            req.flash('success', 'Quiz edited successfully.');
            res.redirect('/quizzes/' + quiz.id);
        })
        .catch(sequelize.ValidationError, error => {
            req.flash('error', 'There are errors in the form:');
            error.errors.forEach(({message}) => req.flash('error', message));
            res.render('quizzes/edit', {quiz});
        })
        .catch(error => {
            req.flash('error', 'Error editing the Quiz: ' + error.message);
            next(error);
        });
};


let availableQuizzes;

// GET /quizzes/randomplay
exports.randomplay = (req, res, next) => {
    if (!req.session.randomPlay) req.session.randomPlay = [];

    models.quiz.count({where: {id: {[sequelize.Op.notIn]: req.session.randomPlay}}})
        .then(count => {
            if (count === 0) {
                const score = req.session.randomPlay.length;
                req.session.randomPlay = [];
                res.render('quizzes/random_none', {//TODO este chunk se puede poner antes de resetar al array para no tener que crear/guarda score en const?
                    score: score
                });
            } else {
                models.quiz.findAll()
                    .then(quizzes => quizzes.map(quiz => quiz.id))
                    .then(ids => ids.filter(id => req.session.randomPlay.indexOf(id) === -1))
                    .then(ids => ids[Math.floor(Math.random() * ids.length)])
                    .then(id => models.quiz.findById(id)
                        .then(quiz => {
                            res.render('quizzes/random_play', {
                                    score: req.session.randomPlay.length,
                                    quiz: quiz
                                }
                            );
                        })).catch(err => console.log(err))
                    .catch(err => console.log(err));
            }
        })

};

// GET /quizzes/randomcheck/:quizId
exports.randomcheck = (req, res, next) => {

    const {quiz, query} = req;

    const answer = query.answer || "";
    const result = answer.toLowerCase().trim() === quiz.answer.toLowerCase().trim();
    let lastScore = req.session.randomPlay.length;

    result ? req.session.randomPlay.push(quiz.id) : req.session.randomPlay = [];

    res.render('quizzes/random_result', {
        answer,
        quiz,
        result,
        score: result ? ++lastScore : lastScore
    });


};

// DELETE /quizzes/:quizId
exports.destroy = (req, res, next) => {

    req.quiz.destroy()
        .then(() => {
            req.flash('success', 'Quiz deleted successfully.');
            res.redirect('/quizzes');
        })
        .catch(error => {
            req.flash('error', 'Error deleting the Quiz: ' + error.message);
            next(error);
        });
};


// GET /quizzes/:quizId/play
exports.play = (req, res, next) => {

    const {quiz, query} = req;

    const answer = query.answer || '';

    res.render('quizzes/play', {
        quiz,
        answer
    });
};


// GET /quizzes/:quizId/check
exports.check = (req, res, next) => {

    const {quiz, query} = req;

    const answer = query.answer || "";
    const result = answer.toLowerCase().trim() === quiz.answer.toLowerCase().trim();

    res.render('quizzes/result', {
        quiz,
        result,
        answer
    });
};
