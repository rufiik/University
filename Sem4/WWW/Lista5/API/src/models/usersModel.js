const pool = require('../database');

const getUsers = async () => {
  return pool.query('SELECT * FROM users');
};
const getUserByUsername = async (username) => {
  const result = await pool.query('SELECT * FROM users WHERE username = ?', [username]);
  return result[0]; // Zwróć pierwszego użytkownika (lub `undefined`, jeśli nie znaleziono)
};
const createUser = async (username, password, role = 'user') => {
  return pool.query('INSERT INTO users (username, password, role) VALUES (?, ?, ?)', [username, password, role]);
};

const deleteUser = async (id) => {
  return pool.query('DELETE FROM users WHERE id = ?', [id]);
};

const updateUser = async (id, username, password, role = 'user') => {
  return pool.query('UPDATE users SET username = ?, password = ?, role = ? WHERE id = ?', [username, password, role, id]);
};
const getUserById = async (id) => {
  const result = await pool.query('SELECT * FROM users WHERE id = ?', [id]);
  return result[0]; // Zwróć pierwszego użytkownika (lub `undefined`)
};

module.exports = { getUsers, createUser, deleteUser, updateUser, getUserByUsername, getUserById };