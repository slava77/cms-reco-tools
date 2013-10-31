ds=combined.tex
fPat=$1

echo "\\documentclass{article}" 
echo "\\usepackage[pdftex]{graphicx,graphics}"

echo "\\begin{document}"
ls ${fPat} | while read -r f; do
    echo "\\begin{figure}"
    echo "\\includegraphics[width=\\textwidth]{${f}}"
    echo "\\caption{"`echo ${f} | sed -e 's/_/\\\\_/g'`"}"
    echo "\\end{figure}"
    echo "\\clearpage"
    echo "\\newpage"
done

echo "\\end{document}"