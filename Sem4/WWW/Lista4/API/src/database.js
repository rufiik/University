const mariadb = require('mariadb');

const pool = mariadb.createPool({
  host: 'localhost',
  user: 'api_user', // Nowy użytkownik
  password: 'api_password', // Hasło nowego użytkownika
  database: 'API',
  connectionLimit: 5,
});

module.exports = pool;