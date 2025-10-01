const { getGames, createGame, deleteGame, updateGame } = require('../models/gamesModel');

const handleGetGames = async (req, res) => {
  try {
    const games = await getGames();
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify(games));
  } catch (error) {
    console.error('Error fetching games:', error);
    res.writeHead(500, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: 'Internal Server Error' }));
  }
};

const handleCreateGame = async (req, res) => {
  let body = '';
  req.on('data', chunk => (body += chunk));
  req.on('end', async () => {
    try {
      const { title, description } = JSON.parse(body);
      if (!title || !description) {
        res.writeHead(400, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Bad Request: Missing required fields' }));
        return;
      }
      await createGame(title, description);
      res.writeHead(201, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ message: 'Game created' }));
    } catch (error) {
      console.error('Error creating game:', error);
      res.writeHead(500, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
  });
};

const handleDeleteGame = async (req, res) => {
  const gameId = req.url.split('/').pop(); 
  try {
    const result = await deleteGame(gameId);
    if (result.affectedRows === 0) {
      res.writeHead(404, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ error: 'Game not found' }));
      return;
    }
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ message: 'Game deleted' }));
  } catch (error) {
    console.error('Error deleting game:', error);
    res.writeHead(500, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: 'Internal Server Error' }));
  }
};

const handleUpdateGame = async (req, res) => {
  const gameId = req.url.split('/').pop();
  let body = '';
  req.on('data', chunk => (body += chunk));
  req.on('end', async () => {
    try {
      const { title, description } = JSON.parse(body);
      if (!title || !description) {
        res.writeHead(400, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Bad Request: Missing required fields' }));
        return;
      }
      const result = await updateGame(gameId, title, description);
      if (result.affectedRows === 0) {
        res.writeHead(404, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Game not found' }));
        return;
      }
      res.writeHead(200, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ message: 'Game updated' }));
    } catch (error) {
      console.error('Error updating game:', error);
      res.writeHead(500, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
  });
};

module.exports = {
  handleGetGames,
  handleCreateGame,
  handleDeleteGame,
  handleUpdateGame,
};