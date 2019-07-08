pipeline {
    agent none

	parameters {
        // Build on Fedora's default compiler as first parallel stages
        booleanParam name: 'Use_gcc8', defaultValue: true, description: 'Build/test using gcc8'
        booleanParam name: 'Use_clang7', defaultValue: true, description: 'Build/test using clang7'
        // Sanitizer tests in parallel stages
        booleanParam name: 'Run_sanitizers', defaultValue: true, description: 'Build/test using sanitizers'
        // Build alternative compilers in parallel stages
        booleanParam name: 'Use_gcc4', defaultValue: true, description: 'Build/test using gcc4'
        booleanParam name: 'Use_gcc5', defaultValue: true, description: 'Build/test using gcc5'
        booleanParam name: 'Use_gcc6', defaultValue: true, description: 'Build/test using gcc6'
        booleanParam name: 'Use_gcc7', defaultValue: true, description: 'Build/test using gcc7'
        booleanParam name: 'Use_gcc9', defaultValue: true, description: 'Build/test using gcc9'
	}
    stages {
        stage('Parallel system compiler stages') {
            failFast true
            parallel {
                stage('gcc8.3.1') {
                    when {
                        environment name: 'Use_gcc8', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "serf-cpp-gcc831:latest"
                        }
                    }
                    steps {
                        // Enable clang-tidy checks on this build and expect clean results
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 8.3.1"
                        dir ("gcc831") {
                            sh 'cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -DENABLE_CLANG_TIDY=TRUE ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }
                
                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc831/unittests.xml'
                        }
                    }
                }
                stage('clang7') {
                    when {
                        environment name: 'Use_clang7', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "serf-cpp-gcc831:latest"
                        }
                    }
                    steps {
                        // Enable cppcheck on this build and expect clean results
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using clang 7"
                        dir ("clang7") {
                            sh 'CC=clang CXX=clang++ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -DENABLE_CPPCHECK=TRUE ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }
                
                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'clang/unittests.xml'
                        }
                    }
                }
            }
        }

        stage('Parallel sanitizer stages') {
            failFast true
            parallel {
                stage('asan') {
                    when {
                        environment name: 'Run_sanitizers', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "serf-cpp-gcc831:latest"
                            args "--cap-add SYS_PTRACE"
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} with asan"
                        dir ("asan") {
                            sh 'cmake -DCMAKE_BUILD_TYPE=asan ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                    }
                }

                stage('tsan') {
                    when {
                        environment name: 'Run_sanitizers', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "serf-cpp-gcc831:latest"
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} with asan"
                        dir ("tsan") {
                            sh 'cmake -DCMAKE_BUILD_TYPE=tsan ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                    }
                }

                stage('ubsan') {
                    when {
                        environment name: 'Run_sanitizers', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "serf-cpp-gcc831:latest"
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} with asan"
                        dir ("ubsan") {
                            sh 'cmake -DCMAKE_BUILD_TYPE=ubsan ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                    }
                }
            }
        }

        stage('Parallel alternate compiler stages') {
            failFast true
            parallel {
                stage('gcc9.1.0') {
                    when {
                        environment name: 'Use_gcc9', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "serf-cpp-gcc910:latest"
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 9.1.0"
                        dir ("gcc910") {
                            sh 'CC=/opt/gcc910/bin/gcc CXX=/opt/gcc910/bin/g++ cmake ..'
                            sh 'LD_LIBRARY_PATH=/opt/gcc910/lib64 make'
                            sh "LD_LIBRARY_PATH=/opt/gcc910/lib64 ./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc910/unittests.xml'
                        }
                    }
                }

                stage('gcc7.4.0') {
                    when {
                        environment name: 'Use_gcc7', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "serf-cpp-gcc740:latest"
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 7.4.0"
                        dir ("gcc740") {
                            sh 'CC=/opt/gcc740/bin/gcc CXX=/opt/gcc740/bin/g++ cmake ..'
                            sh 'LD_LIBRARY_PATH=/opt/gcc740/lib64 make'
                            sh "LD_LIBRARY_PATH=/opt/gcc740/lib64 ./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc740/unittests.xml'
                        }
                    }
                }

                stage('gcc6.5.0') {
                    when {
                        environment name: 'Use_gcc6', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "serf-cpp-gcc650:latest"
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 6.5.0"
                        dir ("gcc650") {
                            sh 'CC=/opt/gcc650/bin/gcc CXX=/opt/gcc650/bin/g++ cmake ..'
                            sh 'LD_LIBRARY_PATH=/opt/gcc650/lib64 make'
                            sh "LD_LIBRARY_PATH=/opt/gcc650/lib64 ./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc650/unittests.xml'
                        }
                    }
                }

                stage('gcc5.3.0') {
                    when {
                        environment name: 'Use_gcc5', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "serf-cpp-gcc530:latest"
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 5.3.0"
                        dir ("gcc530") {
                            sh 'CC=/usr/bin/gcc53 CXX=/usr/bin/g++53 cmake ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc530/unittests.xml'
                        }
                    }
                }

                stage('gcc4.9.3') {
                    when {
                        environment name: 'Use_gcc4', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "serf-cpp-gcc493:latest"
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 4.9.3"
                        dir ("gcc493") {
                            sh 'CC=/usr/bin/gcc49 CXX=/usr/bin/g++49 cmake ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc493/unittests.xml'
                        }
                    }
                }
            }
        }
    }
    
}