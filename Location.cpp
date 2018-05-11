/* Describes the co-ordinates of the cell in the grid */
class Location
{
	private:
		int x;                      // X coordinate of the cell
		int y;                      // Y coordinate of the cell
	public:
		Location(int x, int y);     // Constructor for the class
		int getX();                 // Returns the X coordinate of the cell
		int getY();                 // Returns the Y coordinate of the cell
};

/* Constructor that sets x and y coordinate for the class */
Location::Location(int x, int y)
{
	this->x = x;
	this->y = y;
}

int Location::getX()
{
	return x;
}

int Location::getY()
{
	return y;
}
