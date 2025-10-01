const jwt = require('jsonwebtoken');

// Klucz tajny do podpisywania tokenów 
const SECRET_KEY = 'your_secret_key';

// Middleware do weryfikacji tokenu JWT
const authenticateToken = (req, res, next) => {
  const authHeader = req.headers['authorization'];
  const token = authHeader && authHeader.split(' ')[1]; // Pobierz token z nagłówka Authorization

  if (!token) {
    res.writeHead(401, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: 'Unauthorized: No token provided' }));
    return;
  }

  jwt.verify(token, SECRET_KEY, (err, user) => {
    if (err) {
      res.writeHead(403, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ error: 'Forbidden: Invalid token' }));
      return;
    }
    req.user = user; // Przechowaj dane użytkownika w obiekcie `req`
    next();
  });
};

// Middleware do autoryzacji na podstawie ról
const authorizeRole = (role) => (req, res, next) => {
  if (req.user.role !== role) {
    res.writeHead(403, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: 'Forbidden: Insufficient permissions' }));
    return;
  }
  next();
};

module.exports = { authenticateToken, authorizeRole };