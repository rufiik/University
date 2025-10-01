const jwt = require('jsonwebtoken');
const { getUserByUsername } = require('../models/usersModel'); 
const SECRET_KEY = 'your_secret_key';

// Endpoint do logowania
const handleLogin = async (req, res) => {
  let body = '';
  req.on('data', chunk => (body += chunk));
  req.on('end', async () => {
    try {
      const { username, password } = JSON.parse(body);
      const user = await getUserByUsername(username);

      if (!user || user.password !== password) {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized: Invalid credentials' }));
        return;
      }

      // Generuj token JWT
      const token = jwt.sign(
        { id: user.id, username: user.username, role: user.role }, // Payload
        SECRET_KEY, // Klucz tajny
        { expiresIn: '1h' } // Czas ważności tokenu
      );

      res.writeHead(200, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ token })); 
    } catch (error) {
      console.error('Error during login:', error);
      res.writeHead(500, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
  });
};

module.exports = { handleLogin };