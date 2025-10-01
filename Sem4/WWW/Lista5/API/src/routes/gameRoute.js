const { handleGetGames, handleCreateGame, handleDeleteGame, handleUpdateGame } = require('../controllers/gamesController');
const { authenticateToken, authorizeRole } = require('../authMiddleware');

module.exports = (req, res) => {
  if (req.method === 'GET') {
    authenticateToken(req, res, () => {
      handleGetGames(req, res);
    });
  } else if (req.method === 'POST') {
    authenticateToken(req, res, () => {
      authorizeRole('admin')(req, res, () => {
        handleCreateGame(req, res);
      });
    });
  } else if (req.method === 'DELETE') {
    authenticateToken(req, res, () => {
      authorizeRole('admin')(req, res, () => {
        handleDeleteGame(req, res);
      });
    });
  } else if (req.method === 'PUT') {
    authenticateToken(req, res, () => {
      authorizeRole('admin')(req, res, () => {
        handleUpdateGame(req, res);
      });
    });
  } else {
    res.writeHead(405, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: 'Method Not Allowed' }));
  }
};