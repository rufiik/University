const mariadb = require('mariadb');
const pool = mariadb.createPool({
  host: 'localhost',
  user: 'api_user', // Nowy użytkownik
  password: process.env.API_DB_PASSWORD, // Hasło nowego użytkownika
  database: 'API',
  connectionLimit: 5,
});

module.exports = pool;