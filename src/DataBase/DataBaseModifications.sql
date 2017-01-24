-- If you want your Database to be uptodate USE: 
ALTER TABLE Terrain CHANGE decription description text;
ALTER TABLE Terrain ADD COLUMN type VARCHAR(50) NOT NULL AFTER name;
ALTER TABLE Sessions DROP COLUMN session_id;

-- OR drop your database and create it once more (for creation use file DataBaseScheme.sql):
DROP DATABASE DnD;
