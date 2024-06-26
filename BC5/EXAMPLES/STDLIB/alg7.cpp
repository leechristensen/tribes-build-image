/**************************************************************************
 *
 * alg7.cpp - Illustrate the use of the sort related generic algorithms.
 *    Section 13
 *
 * $Id: alg7.cpp,v 1.2 1995/08/29 18:43:42 oberg Exp $
 *
 * $$RW_INSERT_HEADER "slyrs.cpp"
 *
 **************************************************************************/

# include <vector>
# include <deque>
# include <list>
# include <algorithm>

# include <iostream.h>
using namespace std;

int randomInteger(unsigned int n)
	{ return rand() % n; }
	
int randomValue() { return randomInteger(100); }

ostream_iterator<int> intOut (cout, " ");
		
void sortExample() {
	cout << "Sort algorithms"  << endl;
	
		// fill both a vector and a deque
		// with random integers
	vector<int> aVec(15);
	deque<int> aDec(15);
	generate (aVec.begin(), aVec.end(), randomValue);
	generate (aDec.begin(), aDec.end(), randomValue);
	
		// sort the vector ascending
	sort (aVec.begin(), aVec.end());
	copy (aVec.begin(), aVec.end(), intOut);
	cout << endl;
	
		// sort the deque descending
	sort (aDec.begin(), aDec.end(), greater<int>() );
	copy (aDec.begin(), aDec.end(), intOut);
	cout << endl;
	
		// sort the vector descending?
	sort (aVec.rbegin(), aVec.rend());
	copy (aVec.begin(), aVec.end(), intOut);
	cout << endl;

}

void partial_sort_example() {
	cout << "Partial sort examples" << endl;
	
		// make a vector of 15 random integers
	vector<int> aVec(15);
	generate (aVec.begin(), aVec.end(), randomValue);
	copy (aVec.begin(), aVec.end(), intOut);
	cout << endl;

	
		// partial sort the first seven positions
	partial_sort (aVec.begin(), aVec.begin() + 7, aVec.end());
	copy (aVec.begin(), aVec.end(), intOut);
	cout << endl;

		// make a list of random integers
	list<int> aList(15, 0);
	generate (aList.begin(), aList.end(), randomValue);
	copy (aList.begin(), aList.end(), intOut);
	cout << endl;
	
		// sort only the first seven elements
	vector<int> start(7);
	partial_sort_copy (aList.begin(), aList.end(),
		start.begin(), start.end(), greater<int>());
	copy (start.begin(), start.end(), intOut);
	cout << endl;	
}

void nth_element_example()
	// illustrate the use of the nth_largest function
{
	cout << "Nth largest example" << endl;

		// make a vector of random integers
	vector<int> aVec(10);
	generate (aVec.begin(), aVec.end(), randomValue);
	
		// now find the 5th largest
	vector<int>::iterator nth = aVec.begin() + 4;
	nth_element(aVec.begin(), nth, aVec.end());
	
	cout << "fifth largest is " << *nth << " in" << endl;
	copy (aVec.begin(), aVec.end(), intOut);
	cout << endl;
	
	
}

void binary_search_example ()
	// illustrate the use of the binary search functions
{
	cout << "Binary search example" << endl;
	
		// make an ordered vector of 15 random integers
	vector<int> aVec(15);
	generate (aVec.begin(), aVec.end(), randomValue);
	sort (aVec.begin(), aVec.end());
	copy (aVec.begin(), aVec.end(), intOut),   cout << endl;
	
		// see if it contains an eleven
	if (binary_search(aVec.begin(), aVec.end(), 11))
		cout << "contains an 11" << endl;
	else
		cout << "does not contain an 11"  << endl;
		
		// insert an 11 and a 14
	vector<int>::iterator where;
	where = lower_bound (aVec.begin(), aVec.end(), 11);
	aVec.insert (where, 11);
	
	where = upper_bound (aVec.begin(), aVec.end(), 14);
	aVec.insert (where, 14);
	
	copy (aVec.begin(), aVec.end(), intOut),   cout << endl;
}

void merge_example ()
	// illustrate the use of the merge function
{
	cout << "Merge algorithm examples" << endl;
	
		// make a list and vector of 10 random integers
	vector<int> aVec(10);
	list<int> aList(10, 0);
	generate (aVec.begin(), aVec.end(), randomValue);
	sort (aVec.begin(), aVec.end());
	generate_n (aList.begin(), 10, randomValue);
	aList.sort();
	
		// merge into a vector
	vector<int> vResult (aVec.size() + aList.size());
	merge (aVec.begin(), aVec.end(), aList.begin(), aList.end(),
			vResult.begin());
			
		// merge into a list
	list<int> lResult;
	merge (aVec.begin(), aVec.end(), aList.begin(), aList.end(),
			inserter(lResult, lResult.begin()));
			
		// merge into the output
	merge (aVec.begin(), aVec.end(), aList.begin(), aList.end(),
		ostream_iterator<int>  (cout, " "));
	cout << endl;
}


class iotaGen {
public:
	iotaGen (int c = 0) : current(c) { }
	int operator () () { return current++; }
private:
	int current;
};

void set_example ()
	// illustrate the use of the generic set functions
{
	cout << "Set operations:" << endl;
	
		// make a couple of ordered lists
	list <int> listOne, listTwo;
	generate_n (inserter(listOne, listOne.begin()), 5, iotaGen(1));
	generate_n (inserter(listTwo, listTwo.begin()), 5, iotaGen(3));
	
		// now do the set operations
		// union - 1 2 3 4 5 6 7
	set_union (listOne.begin(), listOne.end(),
		listTwo.begin(), listTwo.end(), intOut), cout << endl;
		// merge - 1 2 3 3 4 4 5 5 6 7
	merge (listOne.begin(), listOne.end(),
		listTwo.begin(), listTwo.end(), intOut), cout << endl;
		// intersection 3 4 5
	set_intersection (listOne.begin(), listOne.end(),
		listTwo.begin(), listTwo.end(), intOut), cout << endl;
		// difference 1 2
	set_difference (listOne.begin(), listOne.end(),
		listTwo.begin(), listTwo.end(), intOut), cout << endl;
		// symmetric difference 1 2 6 7
	set_symmetric_difference (listOne.begin(), listOne.end(),
		listTwo.begin(), listTwo.end(), intOut), cout << endl;
		
	if (includes(listOne.begin(), listOne.end(),
			listTwo.begin(), listTwo.end()))
				cout << "set is subset" << endl;
	else
		cout << "set is not subset" << endl;

}


void heap_example ()
	// illustrate the use of the heap functions
{
	ostream_iterator<int> intOut (cout, " ");
	
		// make a heap of 15 random integers
	vector<int> aVec(15);
	generate (aVec.begin(), aVec.end(), randomValue);
	make_heap (aVec.begin(), aVec.end());
	copy (aVec.begin(), aVec.end(), intOut),   cout << endl;
	
	cout << "Largest value " << aVec.front() << endl;
	
		// remove largest and reheap
	pop_heap(aVec.begin(), aVec.end());
	aVec.pop_back();
	copy (aVec.begin(), aVec.end(), intOut),   cout << endl;
	
		// add a 97 to the heap
	aVec.push_back(97);
	push_heap (aVec.begin(), aVec.end());
	copy (aVec.begin(), aVec.end(), intOut),   cout << endl;
	
		// finally, make into sorted collection
	sort_heap (aVec.begin(), aVec.end());
	copy (aVec.begin(), aVec.end(), intOut),   cout << endl;
}

int main() {
	cout << "Sorting generic algorithms examples" << endl;

	sortExample();
	partial_sort_example();
	nth_element_example();
	binary_search_example();
	merge_example();
	set_example();
	heap_example();
	
	cout << "End sorting examples" << endl;
	return 0;
}
