const { handleGetUsers, handleCreateUser, handleDeleteUser, handleUpdateUser, handleUpdateMe } = require('../controllers/usersController');

module.exports = (req, res) => {
  if (req.method === 'GET') {
    handleGetUsers(req, res);
  } else if (req.method === 'POST') {
    handleCreateUser(req, res); 
  } else if (req.method === 'DELETE') {
    handleDeleteUser(req, res); 
  } else if (req.method === 'PUT') {
        if (req.url.endsWith('/me')) {
      handleUpdateMe(req, res); 
    } else {
      handleUpdateUser(req, res); 
    }
  } else {
    res.writeHead(405, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: 'Method Not Allowed' }));
  }
};