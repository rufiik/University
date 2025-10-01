const pool = require('../database');

const getGames = async () => {
  return pool.query('SELECT * FROM games');
};

const createGame = async (title, description) => {
  return pool.query('INSERT INTO games (title, description) VALUES (?, ?)', [title, description]);
};

const deleteGame = async (id) => {
  return pool.query('DELETE FROM games WHERE id = ?', [id]);
};


const updateGame = async (id, title, description) => {
  return pool.query('UPDATE games SET title = ?, description = ? WHERE id = ?', [title, description, id]);
};

module.exports = { getGames, createGame, deleteGame, updateGame };