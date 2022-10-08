## Pull Request How To
version 0.03
DRAFT, INCOMPLETE AND NOT REVIEWED!

Here are the steps to follow to modify the TriEmbed Edger project repository at https://github.com/TriEmbed/edger


Ordinarily <branchname> below will be of the form issueN where N is the repo issue number belonging to the TriEmbed edger repo that will be merged into.

The overall flow is:
1. Fork https://github.com/TriEmbed/edger into your personal github account
2. Clone the repo from your account to your local computer
3. Make changes related to the issue in a new branch
4. Push the changes to your account's edger repo
5. Use the github pull request steps and go through project Edger ttesting and code review processes
6. Have the edger repo gatekeeper merge your branch (Ron: this is a kind of guess. Merges are a major mystery in git-land!)

The following steps describe the complete process in detail. Send questions and comments to pete@soper.us and roncraig007@gmail.com (Pete Soper and Ron Craig on Signal).

1. In https://github.com/TriEmbed/edger use the "Fork" button to make a fork of the repository in your personal Github account. 
    1. If you see that the target repo name in your personal account has a hyphen and number it means you aleady have one or more forks. This can be a cause of confusion and mistakes. To avoid this use a different name in the text box before completing the fork, such as "issueN" or "issueN-meaningful-label".
2. In your personal account use the "clone" button to create a local clone of the forked repository. The next several steps will all involve this local clone repo.
3. Use command "git checkout -b <branchname>" with "issueN" as the preferred branch name to create the needed branch and make it the current one
4. Make changes to files: the "git add" and "git commit" steps below will often be iterated as changes are applied, corrected, extended, etc. 
    1. To undo a git add use "git reset"
    2. To undo a git commit use "git commit --soft HEAD~n" where n is 1 to undo the most recent commit, 2, to undo the most recent two commits, etc. Use "git log" to confirm the erasures.
    3. In the worst case:
        1. Stash the clone with havoc
        2. Make a fresh clone
        3. Make a branch and check it out in the fresh clone with the steps above
        4. Copy/add/commit files from the havoc repo to the fresh clone to recover. The "git log" command can be directed to a file and used for reference for ereminders about commit details.
5. git add <files>
    1. Tip: You can "git add <directory> to cause a git add of all the directory files (recursive over all subdirectory files too). But be careful about the fact that ALL files not mentioned in the repo .gitignore file will be added. This can cause junk to end up in the repo.
6. git commit -m "commit message" 
    1. The last commit should begin with the text "issueN" where N is the specific issue being resolved. 
    2. To see changes made in a branch use "git log". The "git status" command will not show any changes.
7. Use the command "git push origin <branchname>:main" to push changes to the parent repo in your account.
    1. SPECIAL NOTE: Out in the world both "origin" and "main" might be different labels. This advanced topic might be covered in a future revision of these notes.

The "git push" should create output like this:

remote: Resolving deltas: 100% (1/1), completed with 1 local object.
remote: 
remote: Create a pull request for 'issue25' on GitHub by visiting:
############# This is an example, not a link to visit
remote:      https://github.com/petesoper/edgerdoc/pull/new/issue25
remote: 

8. Go to the specific link in the your account

Versions of this how-to that are designed for popular GUI interfaces to git/github welcome!
