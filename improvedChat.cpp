/* EECS 348 - Assignment 4
Author: Sydney Moroney
Sources: All code created and revised by ChatGPT
Collaborators: None
Creation date: 10/1/26
Revision date: 10/1/26
Purpose: The program stores unread emails in a vector-based max heap.
    Emails are prioritized first by sender category and then by date.
    Higher-priority sender categories appear before lower-priority
    categories. If two emails have the same sender category, the
    newer email receives higher priority.
*/


/*
    Source: OpenAI ChatGPT

    The following header files provide the standard-library tools
    used throughout the program.

    <iostream> provides cout and cerr for console output.
    <fstream> provides ifstream for reading from the input file.
    <sstream> provides ostringstream for rebuilding formatted dates.
    <vector> provides the vector used to store the binary max heap.
    <string> provides the string data type.
    <utility> provides swap().
    <stdexcept> provides the out_of_range exception.
    <cctype> provides isdigit() for validating dates.
    <iomanip> provides setw() and setfill() for date formatting.
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <stdexcept>
#include <cctype>
#include <iomanip>

// Source: OpenAI ChatGPT
// Allows standard-library names such as string and vector to be
// used without placing std:: before every occurrence.
using namespace std;


/*
    Source: OpenAI ChatGPT

    SenderCategory represents the possible sender types.

    An enum is used instead of storing the sender category as a full
    string inside every Email object. This reduces the amount of
    storage required for each Email and allows sender priority to
    be compared using integer values.

    Larger enum values represent higher email priority.

    Boss            = 5, highest priority
    Subordinate     = 4
    Peer            = 3
    ImportantPerson = 2
    OtherPerson     = 1, lowest priority
*/
enum class SenderCategory {
    OtherPerson = 1,
    ImportantPerson = 2,
    Peer = 3,
    Subordinate = 4,
    Boss = 5
};


/*
    Source: OpenAI ChatGPT

    The Email class represents one email stored in the priority queue.

    Each Email stores:
    - sender: the sender category as a SenderCategory enum.
    - subject: the subject line of the email.
    - datePriority: the email date stored numerically as YYYYMMDD.

    For example:
        10-01-2026 becomes 20261001.

    Storing the date numerically allows dates to be compared quickly
    without repeatedly converting the original date string.
*/
class Email {

private:

    // Stores the priority category of the email sender.
    SenderCategory sender;

    // Stores the email's subject line.
    string subject;

    // Stores the date numerically in YYYYMMDD format.
    int datePriority;


public:

    /*
        Source: OpenAI ChatGPT

        Constructor for an Email object.

        The constructor receives the sender category, subject,
        and numeric date and stores those values in the object's
        private data members.
    */
    Email(
        SenderCategory sender,
        const string& subject,
        int datePriority
    )
        : sender(sender),
          subject(subject),
          datePriority(datePriority) {
    }


    /*
        Source: OpenAI ChatGPT

        Returns the sender category of the Email.

        The function is marked const because it only reads data
        and does not modify the Email object.
    */
    SenderCategory getSender() const {

        // Return the stored sender category.
        return sender;
    }


    /*
        Source: OpenAI ChatGPT

        Returns a constant reference to the Email subject.

        Returning a const reference prevents an unnecessary copy
        of the subject string while also preventing the caller from
        modifying the original subject.
    */
    const string& getSubject() const {

        // Return the stored email subject.
        return subject;
    }


    /*
        Source: OpenAI ChatGPT

        Returns the numeric YYYYMMDD representation of the date.

        The function is const because it does not modify the Email.
    */
    int getDatePriority() const {

        // Return the stored numeric date.
        return datePriority;
    }
};


/*
    Source: OpenAI ChatGPT

    trim() removes spaces and tab characters from the beginning
    and end of a string.

    This allows input such as:

        EMAIL Boss, Meeting, 10-01-2026

    to be treated the same as:

        EMAIL Boss,Meeting,10-01-2026
*/
string trim(const string& str) {

    // Find the position of the first character that is not
    // a space or tab.
    size_t start = str.find_first_not_of(" \t");

    // If no non-whitespace character exists, the string
    // contains only whitespace, so return an empty string.
    if (start == string::npos) {
        return "";
    }

    // Find the position of the last character that is not
    // a space or tab.
    size_t end = str.find_last_not_of(" \t");

    // Return only the portion of the string between the first
    // and last non-whitespace characters.
    return str.substr(start, end - start + 1);
}


/*
    Source: OpenAI ChatGPT

    parseSenderCategory() converts a sender category written as
    a string into the matching SenderCategory enum value.

    Parameters:
    - sender contains the sender category from the input.
    - category is passed by reference so this function can store
      the resulting enum value in the caller's variable.

    The function returns true when the sender category is valid.

    The function returns false when the sender category does not
    match one of the five accepted categories.
*/
bool parseSenderCategory(
    const string& sender,
    SenderCategory& category
) {

    // Check whether the sender is the Boss category.
    if (sender == "Boss") {

        // Store the matching enum value.
        category = SenderCategory::Boss;
    }

    // If the sender is not Boss, check for Subordinate.
    else if (sender == "Subordinate") {

        // Store the Subordinate enum value.
        category = SenderCategory::Subordinate;
    }

    // Check whether the sender is Peer.
    else if (sender == "Peer") {

        // Store the Peer enum value.
        category = SenderCategory::Peer;
    }

    // Check whether the sender is ImportantPerson.
    else if (sender == "ImportantPerson") {

        // Store the ImportantPerson enum value.
        category = SenderCategory::ImportantPerson;
    }

    // Check whether the sender is OtherPerson.
    else if (sender == "OtherPerson") {

        // Store the OtherPerson enum value.
        category = SenderCategory::OtherPerson;
    }

    // If none of the valid categories matched, the input
    // contains an invalid sender category.
    else {

        // Tell the caller that conversion failed.
        return false;
    }

    // Reaching this point means a valid category was found.
    return true;
}


/*
    Source: OpenAI ChatGPT

    senderToString() converts a SenderCategory enum value back
    into the text that should appear in program output.

    The Email object stores the sender efficiently as an enum,
    but the program must convert it back to readable text when
    displaying the next email.
*/
string senderToString(SenderCategory sender) {

    // Determine which enum value was passed to the function.
    switch (sender) {

        // Convert Boss to its corresponding string.
        case SenderCategory::Boss:
            return "Boss";

        // Convert Subordinate to its corresponding string.
        case SenderCategory::Subordinate:
            return "Subordinate";

        // Convert Peer to its corresponding string.
        case SenderCategory::Peer:
            return "Peer";

        // Convert ImportantPerson to its corresponding string.
        case SenderCategory::ImportantPerson:
            return "ImportantPerson";

        // Convert OtherPerson to its corresponding string.
        case SenderCategory::OtherPerson:
            return "OtherPerson";
    }

    // This should never occur when the enum contains a valid value,
    // but it provides defensive handling for an unexpected value.
    return "Unknown";
}


/*
    Source: OpenAI ChatGPT

    isLeapYear() determines whether a given year is a leap year.

    A year is a leap year if:
    - It is divisible by 400, OR
    - It is divisible by 4 but not divisible by 100.

    This function is used during date validation to determine
    whether February may contain 29 days.
*/
bool isLeapYear(int year) {

    // Return true when the year satisfies the leap-year rules.
    return (year % 400 == 0) ||
           (year % 4 == 0 && year % 100 != 0);
}


/*
    Source: OpenAI ChatGPT

    parseDate() validates a date written in MM-DD-YYYY format
    and converts the date into one integer written conceptually
    as YYYYMMDD.

    Example:
        "10-01-2026" becomes 20261001.

    Parameters:
    - date contains the date string from the input.
    - datePriority is passed by reference and receives the
      converted integer if the date is valid.

    The function returns true for a valid date and false for
    an invalid date.
*/
bool parseDate(
    const string& date,
    int& datePriority
) {

    // A properly formatted MM-DD-YYYY date must contain
    // exactly ten characters.
    if (date.length() != 10) {

        // Reject the date if its length is incorrect.
        return false;
    }

    // Character positions 2 and 5 must contain dashes.
    if (date[2] != '-' || date[5] != '-') {

        // Reject dates that do not use MM-DD-YYYY formatting.
        return false;
    }

    // Examine every character in the date.
    for (size_t i = 0; i < date.length(); i++) {

        // Positions 2 and 5 were already verified as dashes,
        // so they do not need to be checked as digits.
        if (i == 2 || i == 5) {
            continue;
        }

        // Every other character must contain a numeric digit.
        if (!isdigit(static_cast<unsigned char>(date[i]))) {

            // Reject the date if a non-digit appears.
            return false;
        }
    }

    // Extract characters 0 and 1 and convert them into
    // the numeric month.
    int month = stoi(date.substr(0, 2));

    // Extract characters 3 and 4 and convert them into
    // the numeric day.
    int day = stoi(date.substr(3, 2));

    // Extract characters 6 through 9 and convert them into
    // the numeric year.
    int year = stoi(date.substr(6, 4));

    // Reject non-positive years.
    if (year <= 0) {
        return false;
    }

    // Valid months range from January, 1, through December, 12.
    if (month < 1 || month > 12) {
        return false;
    }

    /*
        Store the normal number of days in each month.

        Index 0 corresponds to January.
        Index 1 corresponds to February.
        ...
        Index 11 corresponds to December.
    */
    int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    // If this is a leap year, February contains 29 days
    // instead of 28.
    if (isLeapYear(year)) {
        daysInMonth[1] = 29;
    }

    // Check whether the day is at least 1 and does not exceed
    // the maximum number of days allowed in the given month.
    if (day < 1 || day > daysInMonth[month - 1]) {

        // Reject impossible dates such as 02-31-2026.
        return false;
    }

    /*
        Convert the date into YYYYMMDD format.

        Multiplying the year by 10000 moves it into the first
        four digits.

        Multiplying the month by 100 moves it into the next
        two digits.

        The day occupies the final two digits.
    */
    datePriority =
        year * 10000 +
        month * 100 +
        day;

    // Reaching this point means the date is valid.
    return true;
}


/*
    Source: OpenAI ChatGPT

    dateToString() converts a numeric YYYYMMDD date back into
    MM-DD-YYYY format for output.

    The program stores dates numerically to reduce storage and
    speed up comparisons. This function reconstructs the
    human-readable representation only when the date must be printed.
*/
string dateToString(int datePriority) {

    // Integer division by 10000 extracts the four-digit year.
    int year = datePriority / 10000;

    /*
        Divide by 100 to remove the day, then use modulo 100
        to isolate the two-digit month.
    */
    int month = (datePriority / 100) % 100;

    // Modulo 100 isolates the final two digits, which contain
    // the day.
    int day = datePriority % 100;

    // Create an output string stream so the date can be
    // formatted before being returned.
    ostringstream output;

    /*
        setfill('0') causes unused width positions to contain zeroes.

        setw(2) makes the month occupy two positions.

        setw(2) makes the day occupy two positions.

        setw(4) makes the year occupy four positions.

        The dash characters recreate MM-DD-YYYY formatting.
    */
    output
        << setfill('0')
        << setw(2) << month
        << "-"
        << setw(2) << day
        << "-"
        << setw(4) << year;

    // Convert the completed output stream into a string
    // and return it.
    return output.str();
}


/*
    Source: OpenAI ChatGPT

    MaxHeap manages the priority queue of unread emails.

    The heap is stored inside a vector.

    For any element at index i:
        Left child  = 2 * i + 1
        Right child = 2 * i + 2
        Parent      = (i - 1) / 2

    The highest-priority Email is always stored at index 0.
*/
class MaxHeap {

private:

    // Stores all unread Email objects in binary max-heap order.
    vector<Email> heap;


    /*
        Source: OpenAI ChatGPT

        higherPriority() contains all priority comparison rules
        in one location.

        Email priority is determined in this order:

        1. Sender category.
        2. Date, if the sender categories are equal.

        Centralizing this logic makes the program easier to maintain.
        If priority rules change later, only this function needs to
        be modified.
    */
    bool higherPriority(
        const Email& email1,
        const Email& email2
    ) const {

        /*
            Convert email1's enum value to an integer.

            Because the enum values were assigned according to
            priority, a larger integer means higher sender priority.
        */
        int sender1 =
            static_cast<int>(email1.getSender());

        // Convert email2's sender enum into its integer priority.
        int sender2 =
            static_cast<int>(email2.getSender());

        // If the sender categories differ, sender category alone
        // determines which email has higher priority.
        if (sender1 != sender2) {

            // Return true when email1's sender category has a
            // larger priority value than email2's.
            return sender1 > sender2;
        }

        /*
            If both emails belong to the same sender category,
            compare the numeric dates.

            Because dates are stored as YYYYMMDD, a larger
            integer represents a newer date.
        */
        return email1.getDatePriority()
             > email2.getDatePriority();
    }


    /*
        Source: OpenAI ChatGPT

        heapifyUp() restores max-heap ordering after a new Email
        is inserted at the end of the vector.

        The inserted Email repeatedly compares itself with its
        parent. If the inserted Email has higher priority, the
        two are swapped.

        This continues until:
        - The Email reaches the root, OR
        - Its parent already has equal or higher priority.

        Worst-case execution time: O(log n).
    */
    void heapifyUp(size_t index) {

        // Continue while the Email is not currently at the root.
        while (index > 0) {

            // Calculate the vector index of the current
            // Email's parent.
            size_t parent =
                (index - 1) / 2;

            // Determine whether the current Email should appear
            // above its parent in the max heap.
            if (higherPriority(
                    heap[index],
                    heap[parent]
                )) {

                // Exchange the current Email with its parent.
                swap(
                    heap[index],
                    heap[parent]
                );

                // Update index because the Email has moved to
                // its parent's previous position.
                index = parent;
            }

            // If the parent already has sufficient priority,
            // max-heap ordering has been restored.
            else {

                // Exit the loop.
                break;
            }
        }
    }


    /*
        Source: OpenAI ChatGPT

        heapifyDown() restores max-heap ordering after the root
        Email has been removed.

        The element placed at the root is compared with both
        children. If one child has higher priority, the element
        swaps with the highest-priority child.

        The process continues until the heap property is restored.

        Worst-case execution time: O(log n).
    */
    void heapifyDown(size_t index) {

        // Store the current number of Emails so repeated calls
        // to heap.size() are unnecessary inside the loop.
        size_t heapSize =
            heap.size();

        // Continue until the current Email no longer needs
        // to move downward.
        while (true) {

            // Calculate the vector index of the left child.
            size_t leftChild =
                2 * index + 1;

            // Calculate the vector index of the right child.
            size_t rightChild =
                2 * index + 2;

            /*
                Assume initially that the current Email has the
                greatest priority.

                This value may be changed if one of the children
                has higher priority.
            */
            size_t largest =
                index;

            // Check that the left child exists and determine
            // whether it has higher priority than the current
            // highest-priority candidate.
            if (
                leftChild < heapSize &&
                higherPriority(
                    heap[leftChild],
                    heap[largest]
                )
            ) {

                // Make the left child the new candidate.
                largest = leftChild;
            }

            // Check that the right child exists and determine
            // whether it has higher priority than the current
            // highest-priority candidate.
            if (
                rightChild < heapSize &&
                higherPriority(
                    heap[rightChild],
                    heap[largest]
                )
            ) {

                // Make the right child the new candidate.
                largest = rightChild;
            }

            // If one of the children has higher priority than
            // the current Email, a swap is required.
            if (largest != index) {

                // Swap the current Email with the child that
                // has the highest priority.
                swap(
                    heap[index],
                    heap[largest]
                );

                // Continue heapifying from the new position.
                index = largest;
            }

            // If neither child has higher priority, the max-heap
            // property has been restored.
            else {

                // Exit the loop.
                break;
            }
        }
    }


public:

    /*
        Source: OpenAI ChatGPT

        insert() adds a new Email to the max heap.

        The Email is first placed at the end of the vector.
        heapifyUp() then restores proper max-heap ordering.

        Worst-case execution time: O(log n).
    */
    void insert(const Email& email) {

        // Append the Email to the end of the vector.
        heap.push_back(email);

        /*
            The newly inserted Email is located at the final
            vector index, which is heap.size() - 1.

            Start heapifying from that position.
        */
        heapifyUp(
            heap.size() - 1
        );
    }


    /*
        Source: OpenAI ChatGPT

        getMax() returns the highest-priority Email without
        removing it from the heap.

        Because this is a max heap, the highest-priority Email
        is always stored at index 0.

        The function returns a const reference to avoid making
        an unnecessary copy of the Email.

        Execution time: O(1).
    */
    const Email& getMax() const {

        // Protect against attempting to access index 0 when
        // the heap contains no Emails.
        if (heap.empty()) {

            // Throw an exception to report improper use of
            // getMax() on an empty heap.
            throw out_of_range(
                "Cannot get an email from an empty heap."
            );
        }

        // Return the root Email.
        return heap[0];
    }


    /*
        Source: OpenAI ChatGPT

        removeMax() removes the highest-priority Email.

        The final Email in the vector is moved into the root
        position. The final vector element is then removed.

        heapifyDown() restores max-heap ordering.

        Worst-case execution time: O(log n).
    */
    void removeMax() {

        // If the heap is already empty, there is nothing
        // to remove.
        if (heap.empty()) {

            // Exit the function safely.
            return;
        }

        /*
            Copy the final Email into the root position.

            This temporarily replaces the highest-priority Email
            before the final element is removed.
        */
        heap[0] =
            heap.back();

        // Remove the final Email from the vector.
        heap.pop_back();

        /*
            If the heap still contains Emails, restore max-heap
            ordering beginning at the root.

            This check prevents heapifyDown() from operating
            unnecessarily on an empty heap.
        */
        if (!heap.empty()) {
            heapifyDown(0);
        }
    }


    /*
        Source: OpenAI ChatGPT

        isEmpty() reports whether the heap currently contains
        any unread Emails.

        Execution time: O(1).
    */
    bool isEmpty() const {

        // Return true when the vector has no elements.
        return heap.empty();
    }


    /*
        Source: OpenAI ChatGPT

        size() returns the number of unread Emails currently
        stored in the heap.

        size_t is used because vector::size() naturally returns
        an unsigned size_t value.

        Execution time: O(1).
    */
    size_t size() const {

        // Return the number of elements in the vector.
        return heap.size();
    }
};


/*
    Source: OpenAI ChatGPT

    parseEmailCommand() extracts and validates all data from
    one EMAIL command.

    Expected format:

        EMAIL SenderCategory,Subject,MM-DD-YYYY

    Parameters:
    - line contains the full input command.
    - sender receives the parsed SenderCategory.
    - subject receives the parsed subject.
    - datePriority receives the numeric YYYYMMDD date.

    The function returns true when the entire command is valid.
    It returns false when the command contains invalid or
    missing data.

    Separating this work into a helper function reduces the
    amount of logic inside main().
*/
bool parseEmailCommand(
    const string& line,
    SenderCategory& sender,
    string& subject,
    int& datePriority
) {

    /*
        "EMAIL " occupies six characters.

        A valid command must contain additional information
        after those six characters.
    */
    if (line.length() <= 6) {

        // Report that no email information followed EMAIL.
        cerr
            << "Error: EMAIL command is missing data."
            << endl;

        // Tell the caller that parsing failed.
        return false;
    }

    /*
        Remove the first six characters:

            EMAIL

        plus the following space.

        The remaining string should contain only the email fields.
    */
    string emailData =
        line.substr(6);

    /*
        Locate the first comma.

        Everything before this comma represents the sender category.
    */
    size_t firstComma =
        emailData.find(',');

    /*
        Locate the final comma.

        Everything after this comma represents the date.

        Using the final comma instead of simply looking for the
        second comma allows the subject itself to contain commas.
    */
    size_t lastComma =
        emailData.rfind(',');

    /*
        The command is invalid if:
        - no first comma exists,
        - no last comma exists, OR
        - the first and last comma are actually the same comma.

        A valid EMAIL command requires at least two commas.
    */
    if (
        firstComma == string::npos ||
        lastComma == string::npos ||
        firstComma == lastComma
    ) {

        // Report the malformed EMAIL command.
        cerr
            << "Error: Invalid EMAIL format."
            << endl;

        // Tell the caller parsing failed.
        return false;
    }

    /*
        Extract the sender field from the beginning of emailData
        through the character immediately before the first comma.

        trim() removes unnecessary surrounding spaces or tabs.
    */
    string senderString =
        trim(
            emailData.substr(
                0,
                firstComma
            )
        );

    /*
        Extract the subject from the character after the first
        comma through the character before the final comma.

        Using the first and last commas allows commas inside
        the subject line.
    */
    subject =
        trim(
            emailData.substr(
                firstComma + 1,
                lastComma - firstComma - 1
            )
        );

    /*
        Extract everything after the final comma.

        This portion should contain the MM-DD-YYYY date.
    */
    string dateString =
        trim(
            emailData.substr(
                lastComma + 1
            )
        );

    /*
        Reject the command if any required field is empty.

        A valid Email must have:
        - a sender category,
        - a subject,
        - a date.
    */
    if (
        senderString.empty() ||
        subject.empty() ||
        dateString.empty()
    ) {

        // Report the missing field.
        cerr
            << "Error: EMAIL fields cannot be empty."
            << endl;

        // Tell the caller parsing failed.
        return false;
    }

    /*
        Convert the sender string into a SenderCategory enum.

        If the sender name is not one of the five accepted values,
        parseSenderCategory() returns false.
    */
    if (!parseSenderCategory(
            senderString,
            sender
        )) {

        // Report the invalid sender name.
        cerr
            << "Error: Invalid sender category: "
            << senderString
            << endl;

        // Tell the caller parsing failed.
        return false;
    }

    /*
        Validate the date and convert it into numeric YYYYMMDD form.

        parseDate() returns false when the date is malformed or
        impossible.
    */
    if (!parseDate(
            dateString,
            datePriority
        )) {

        // Report the invalid date.
        cerr
            << "Error: Invalid date: "
            << dateString
            << endl;

        // Tell the caller parsing failed.
        return false;
    }

    // Every field was successfully parsed and validated.
    return true;
}


/*
    Source: OpenAI ChatGPT

    printNextEmail() displays the highest-priority unread Email
    without removing it from the heap.

    Keeping output in a separate function improves readability
    and keeps output formatting out of main().
*/
void printNextEmail(
    const MaxHeap& emailQueue
) {

    // If the heap is empty, there is no Email to display.
    if (emailQueue.isEmpty()) {

        // Exit the function without attempting to access the heap.
        return;
    }

    /*
        Retrieve a constant reference to the highest-priority Email.

        A reference avoids creating an unnecessary copy.
    */
    const Email& email =
        emailQueue.getMax();

    // Print the heading required before displaying an Email.
    cout
        << "Next email:"
        << endl;

    /*
        Convert the stored SenderCategory enum into readable text
        and display it.
    */
    cout
        << "\tSender: "
        << senderToString(
               email.getSender()
           )
        << endl;

    // Display the Email's subject line.
    cout
        << "\tSubject: "
        << email.getSubject()
        << endl;

    /*
        Convert the stored numeric date back into MM-DD-YYYY
        format and display it.
    */
    cout
        << "\tDate: "
        << dateToString(
               email.getDatePriority()
           )
        << endl;

    // Print a blank line after the Email for readable output.
    cout
        << endl;
}


/*
    Source: OpenAI ChatGPT

    processCommand() handles one complete input command.

    Moving command processing into this helper function keeps
    main() short and improves maintainability.

    The function recognizes four valid commands:

        EMAIL
        NEXT
        READ
        COUNT

    The MaxHeap is passed by reference so this function can
    insert and remove Emails from the original queue.
*/
void processCommand(
    const string& line,
    MaxHeap& emailQueue
) {

    /*
        Remove surrounding whitespace temporarily and determine
        whether the input line contains any meaningful characters.
    */
    if (trim(line).empty()) {

        // Ignore blank or whitespace-only lines.
        return;
    }

    /*
        rfind("EMAIL ", 0) checks whether "EMAIL " begins at
        position 0 of the line.

        If so, this is an EMAIL insertion command.
    */
    if (line.rfind("EMAIL ", 0) == 0) {

        // Variable that will receive the parsed sender category.
        SenderCategory sender;

        // Variable that will receive the parsed subject.
        string subject;

        // Variable that will receive the numeric date.
        int datePriority;

        /*
            Attempt to parse and validate the entire EMAIL command.

            The command should only be inserted into the heap
            when parsing succeeds.
        */
        if (
            parseEmailCommand(
                line,
                sender,
                subject,
                datePriority
            )
        ) {

            /*
                Construct an Email object using the validated
                sender, subject, and date.
            */
            Email newEmail(
                sender,
                subject,
                datePriority
            );

            // Insert the new Email into the max heap.
            emailQueue.insert(
                newEmail
            );
        }
    }

    /*
        If the command is NEXT, display the highest-priority
        Email without removing it.
    */
    else if (line == "NEXT") {

        // Only attempt to print when at least one Email exists.
        if (!emailQueue.isEmpty()) {

            // Display the highest-priority Email.
            printNextEmail(
                emailQueue
            );
        }
    }

    /*
        If the command is READ, remove the highest-priority Email.
    */
    else if (line == "READ") {

        // Only remove an Email if the queue is not empty.
        if (!emailQueue.isEmpty()) {

            // Remove the root of the max heap.
            emailQueue.removeMax();
        }
    }

    /*
        If the command is COUNT, report how many unread Emails
        currently remain in the heap.
    */
    else if (line == "COUNT") {

        // Print the number of stored Emails.
        cout
            << "There are "
            << emailQueue.size()
            << " emails to read.\n"
            << endl;
    }
}


/*
    Source: OpenAI ChatGPT

    main() controls the overall program.

    Its responsibilities are intentionally limited to:

    1. Verify that an input filename was provided.
    2. Open the input file.
    3. Create the email priority queue.
    4. Read each command from the file.
    5. Send each command to processCommand().
    6. Close the file.
    7. End the program.

    Moving parsing and command logic into helper functions keeps
    main() short, readable, and maintainable.
*/
int main(
    int argc,
    char* argv[]
) {

    /*
        argc stores the number of command-line arguments.

        A value below 2 means the user did not provide a filename.
    */
    if (argc < 2) {

        // Print an error explaining what information is missing.
        cerr
            << "Error: Please provide a test file."
            << endl;

        // Return 1 to indicate that the program ended because
        // of an error.
        return 1;
    }

    /*
        argv[1] contains the filename supplied by the user.

        Create an input file stream and attempt to open that file.
    */
    ifstream inputFile(
        argv[1]
    );

    // Check whether the file failed to open.
    if (!inputFile) {

        // Report the file-opening failure.
        cerr
            << "Error: Could not open file."
            << endl;

        // End the program with an error status.
        return 1;
    }

    /*
        Create the max heap that will store all unread Emails
        while commands are processed.
    */
    MaxHeap emailQueue;

    /*
        This string temporarily stores one complete line
        read from the input file.
    */
    string line;

    /*
        getline() reads one complete line at a time from inputFile.

        The loop continues until the end of the file is reached
        or input reading fails.
    */
    while (
        getline(
            inputFile,
            line
        )
    ) {

        /*
            Pass the line to processCommand().

            That helper determines which command was supplied
            and performs the appropriate action.
        */
        processCommand(
            line,
            emailQueue
        );
    }

    // Explicitly close the input file after all commands
    // have been processed.
    inputFile.close();

    // Return 0 to indicate successful program completion.
    return 0;
}